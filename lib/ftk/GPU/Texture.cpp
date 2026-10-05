// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/Texture.h>

#include <ftk/GPU/Render.h>
#include <ftk/GPU/System.h>

#include <ftk/Core/Format.h>

#include <SDL3/SDL.h>

#include <algorithm>

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace ftk
{
    namespace gpu
    {
        namespace
        {
            SDL_GPUTextureFormat getFormat(ImageType value)
            {
                SDL_GPUTextureFormat out = SDL_GPU_TEXTUREFORMAT_INVALID;
                switch (value)
                {
                case ImageType::L_U8: out = SDL_GPU_TEXTUREFORMAT_R8_UNORM; break;
                case ImageType::L_U16: out = SDL_GPU_TEXTUREFORMAT_R16_UNORM; break;
                case ImageType::L_F16: out = SDL_GPU_TEXTUREFORMAT_R16_FLOAT; break;
                case ImageType::L_F32: out = SDL_GPU_TEXTUREFORMAT_R32_FLOAT; break;
                case ImageType::LA_U8: out = SDL_GPU_TEXTUREFORMAT_R8G8_UNORM; break;
                case ImageType::LA_U16: out = SDL_GPU_TEXTUREFORMAT_R16G16_UNORM; break;
                case ImageType::LA_F16: out = SDL_GPU_TEXTUREFORMAT_R16G16_FLOAT; break;
                case ImageType::LA_F32: out = SDL_GPU_TEXTUREFORMAT_R32G32_FLOAT; break;
                case ImageType::RGB_U8:
                case ImageType::RGBA_U8: out = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM; break;
                case ImageType::RGB_U16:
                case ImageType::RGBA_U16: out = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM; break;
                case ImageType::RGB_F16:
                case ImageType::RGBA_F16: out = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT; break;
                case ImageType::RGB_F32:
                case ImageType::RGBA_F32: out = SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT; break;
                default: break;
                }
                return out;
            }

            // The bytes of a channel, and what a fourth channel that was
            // not there is given: one, in the channel's own type.
            size_t getChannelByteCount(ImageType value)
            {
                return getBitDepth(value) / 8;
            }

            void opaque(ImageType type, uint8_t* p)
            {
                switch (type)
                {
                case ImageType::RGB_U8: p[0] = 255; break;
                case ImageType::RGB_U16: { const uint16_t v = 65535; std::memcpy(p, &v, 2); break; }
                case ImageType::RGB_F16: { const uint16_t v = 0x3C00; std::memcpy(p, &v, 2); break; }
                case ImageType::RGB_F32: { const float v = 1.F; std::memcpy(p, &v, 4); break; }
                default: break;
                }
            }
        }

        namespace
        {
            // Sixteen bit normalized textures are the ones Vulkan leaves to
            // the driver. Where there are none they are kept as half float,
            // which every driver has and filters: the shaders read the same
            // values, to the eleven bits or so a half has near one, where
            // sixteen bit integers have sixteen.
            SDL_GPUTextureFormat getHalfFormat(SDL_GPUTextureFormat value)
            {
                SDL_GPUTextureFormat out = SDL_GPU_TEXTUREFORMAT_INVALID;
                switch (value)
                {
                case SDL_GPU_TEXTUREFORMAT_R16_UNORM: out = SDL_GPU_TEXTUREFORMAT_R16_FLOAT; break;
                case SDL_GPU_TEXTUREFORMAT_R16G16_UNORM: out = SDL_GPU_TEXTUREFORMAT_R16G16_FLOAT; break;
                case SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM: out = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT; break;
                default: break;
                }
                return out;
            }

            // The half float thirty-two bit float formats are kept as
            // where a device does not filter them; see hasFloatFilter().
            SDL_GPUTextureFormat getHalfFloatFormat(SDL_GPUTextureFormat value)
            {
                SDL_GPUTextureFormat out = SDL_GPU_TEXTUREFORMAT_INVALID;
                switch (value)
                {
                case SDL_GPU_TEXTUREFORMAT_R32_FLOAT: out = SDL_GPU_TEXTUREFORMAT_R16_FLOAT; break;
                case SDL_GPU_TEXTUREFORMAT_R32G32_FLOAT: out = SDL_GPU_TEXTUREFORMAT_R16G16_FLOAT; break;
                case SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT: out = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT; break;
                default: break;
                }
                return out;
            }

            // A float as a half float, to the nearest.
            uint16_t toHalf(float value)
            {
                uint32_t bits = 0;
                std::memcpy(&bits, &value, 4);
                const uint16_t sign = static_cast<uint16_t>((bits >> 16) & 0x8000);
                const uint32_t exponentBits = (bits >> 23) & 0xFF;
                uint32_t mantissa = bits & 0x7FFFFF;
                if (0xFF == exponentBits)
                {
                    // Infinity, and what is not a number.
                    return sign | (mantissa ? 0x7E00 : 0x7C00);
                }
                const int exponent = static_cast<int>(exponentBits) - 127 + 15;
                if (exponent >= 31)
                {
                    // Too large: the largest there is, not infinity.
                    return sign | 0x7BFF;
                }
                if (exponent <= 0)
                {
                    // Too small to be normal in a half.
                    if (exponent < -10)
                    {
                        return sign;
                    }
                    mantissa |= 0x800000;
                    const int shift = 14 - exponent;
                    uint32_t out = mantissa >> shift;
                    if ((mantissa >> (shift - 1)) & 1)
                    {
                        ++out;
                    }
                    return sign | static_cast<uint16_t>(out);
                }
                uint32_t out = (static_cast<uint32_t>(exponent) << 10) | (mantissa >> 13);
                if (mantissa & 0x1000)
                {
                    // A carry out of the mantissa is the next exponent,
                    // and out of the last exponent would be infinity.
                    out = std::min(out + 1, static_cast<uint32_t>(0x7BFF));
                }
                return sign | static_cast<uint16_t>(out);
            }

            // Every sixteen bit value as the half float it is kept as.
            const std::vector<uint16_t>& getHalfTable()
            {
                static const std::vector<uint16_t> table = []
                {
                    std::vector<uint16_t> out(65536);
                    for (size_t i = 0; i < out.size(); ++i)
                    {
                        out[i] = toHalf(i / 65535.F);
                    }
                    return out;
                }();
                return table;
            }
        }

        void floatToHalf(const float* in, uint16_t* out, size_t count)
        {
            for (size_t i = 0; i < count; ++i)
            {
                out[i] = toHalf(in[i]);
            }
        }

        bool hasUNorm16(SDL_GPUDevice* device)
        {
            // Asked for by name to try what a driver without them gets.
            static const bool no = getEnvFlag("FTK_GPU_NO_UNORM16");
            return !no &&
                SDL_GPUTextureSupportsFormat(
                    device,
                    SDL_GPU_TEXTUREFORMAT_R16_UNORM,
                    SDL_GPU_TEXTURETYPE_2D,
                    SDL_GPU_TEXTUREUSAGE_SAMPLER) &&
                SDL_GPUTextureSupportsFormat(
                    device,
                    SDL_GPU_TEXTUREFORMAT_R16G16_UNORM,
                    SDL_GPU_TEXTURETYPE_2D,
                    SDL_GPU_TEXTUREUSAGE_SAMPLER) &&
                SDL_GPUTextureSupportsFormat(
                    device,
                    SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM,
                    SDL_GPU_TEXTURETYPE_2D,
                    SDL_GPU_TEXTUREUSAGE_SAMPLER);
        }

        namespace
        {
            std::atomic<size_t> objectCount = 0;
            std::atomic<size_t> totalByteCount = 0;

            // What a texture takes on the GPU, where three channels are
            // kept as four.
            size_t getGPUByteCount(const ImageInfo& info)
            {
                const int channels = getChannelCount(info.type);
                return static_cast<size_t>(info.size.w) * info.size.h *
                    (3 == channels ? 4 : channels) *
                    getChannelByteCount(info.type);
            }
        }

        bool isTextureSupported(ImageType value)
        {
            return getFormat(value) != SDL_GPU_TEXTUREFORMAT_INVALID;
        }

        struct Texture::Private
        {
            std::shared_ptr<System> system;
            ImageInfo info;
            SDL_GPUTexture* texture = nullptr;
            SDL_GPUSampler* sampler = nullptr;
            bool counted = false;
            //! Sixteen bit normalized, kept as half float: see hasUNorm16().
            bool half = false;
            //! Thirty-two bit float, kept as half float: see
            //! hasFloatFilter().
            bool halfFloat = false;
            //! The transfer buffer the copies go through; see prepare().
            SDL_GPUTransferBuffer* transfer = nullptr;
            size_t transferByteCount = 0;
        };

        void Texture::_init(
            const std::shared_ptr<System>& system,
            const ImageInfo& info,
            const TextureOptions& options)
        {
            FTK_P();
            p.system = system;
            p.info = info;
            SDL_GPUDevice* device = system->getDevice();

            SDL_GPUTextureFormat format = getFormat(info.type);
            if (SDL_GPU_TEXTUREFORMAT_INVALID == format || !info.isValid())
            {
                throw std::runtime_error(Format("Cannot create a texture: {0}").arg(getLabel(info)));
            }
            if (getHalfFormat(format) != SDL_GPU_TEXTUREFORMAT_INVALID && !hasUNorm16(device))
            {
                format = getHalfFormat(format);
                p.half = true;
            }
            else if (getHalfFloatFormat(format) != SDL_GPU_TEXTUREFORMAT_INVALID && !hasFloatFilter(system))
            {
                format = getHalfFloatFormat(format);
                p.halfFloat = true;
            }
            SDL_GPUTextureCreateInfo textureInfo = {};
            textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
            textureInfo.format = format;
            textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
            textureInfo.width = info.size.w;
            textureInfo.height = info.size.h;
            textureInfo.layer_count_or_depth = 1;
            textureInfo.num_levels = 1;
            p.texture = SDL_CreateGPUTexture(device, &textureInfo);
            if (!p.texture)
            {
                throw std::runtime_error(Format("Cannot create a texture: {0}").arg(SDL_GetError()));
            }

            // The two pass resample weighs texels itself, as it does in the
            // OpenGL renderer; here it is drawn as nearest for now.
            const auto filter = [](ImageFilter value)
            {
                return ImageFilter::Linear == value ?
                    SDL_GPU_FILTER_LINEAR :
                    SDL_GPU_FILTER_NEAREST;
            };
            SDL_GPUSamplerCreateInfo samplerInfo = {};
            samplerInfo.min_filter = filter(options.filters.minify);
            samplerInfo.mag_filter = filter(options.filters.magnify);
            samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
            samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            p.sampler = SDL_CreateGPUSampler(device, &samplerInfo);
            if (!p.sampler)
            {
                throw std::runtime_error(Format("Cannot create a sampler: {0}").arg(SDL_GetError()));
            }
            ++objectCount;
            totalByteCount += getGPUByteCount(p.info);
            p.counted = true;
        }

        Texture::Texture() :
            _p(new Private)
        {}

        Texture::~Texture()
        {
            FTK_P();
            if (p.counted)
            {
                --objectCount;
                totalByteCount -= getGPUByteCount(p.info);
            }
            if (p.system)
            {
                SDL_GPUDevice* device = p.system->getDevice();
                if (p.sampler)
                {
                    SDL_ReleaseGPUSampler(device, p.sampler);
                }
                if (p.texture)
                {
                    SDL_ReleaseGPUTexture(device, p.texture);
                }
                if (p.transfer)
                {
                    SDL_ReleaseGPUTransferBuffer(device, p.transfer);
                }
            }
        }

        std::shared_ptr<Texture> Texture::create(
            const std::shared_ptr<System>& system,
            const ImageInfo& info,
            const TextureOptions& options)
        {
            auto out = std::shared_ptr<Texture>(new Texture);
            out->_init(system, info, options);
            return out;
        }

        size_t Texture::getObjectCount()
        {
            return objectCount;
        }

        size_t Texture::getTotalByteCount()
        {
            return totalByteCount;
        }

        const ImageInfo& Texture::getInfo() const
        {
            return _p->info;
        }

        SDL_GPUTexture* Texture::getTexture() const
        {
            return _p->texture;
        }

        SDL_GPUSampler* Texture::getSampler() const
        {
            return _p->sampler;
        }

        void Texture::copy(const std::shared_ptr<Image>& image)
        {
            _copy(prepare(image));
        }

        void Texture::copy(const uint8_t* data, const ImageInfo& info)
        {
            _copy(prepare(data, info));
        }

        void Texture::copy(const std::shared_ptr<Image>& image, int x, int y)
        {
            _copy(prepare(image, x, y));
        }

        Texture::Upload Texture::prepare(const std::shared_ptr<Image>& image)
        {
            return _prepare(image->getData(), image->getInfo(), 0, 0, true);
        }

        Texture::Upload Texture::prepare(const uint8_t* data, const ImageInfo& info)
        {
            return _prepare(data, info, 0, 0, true);
        }

        Texture::Upload Texture::prepare(const std::shared_ptr<Image>& image, int x, int y)
        {
            return _prepare(image->getData(), image->getInfo(), x, y, false);
        }

        void Texture::send(SDL_GPUDevice* device, SDL_GPUCopyPass* pass, const Upload& value)
        {
            if (!value.transfer)
                return;
            SDL_GPUTextureTransferInfo source = {};
            source.transfer_buffer = value.transfer;
            SDL_GPUTextureRegion region = {};
            region.texture = value.texture;
            region.x = value.x;
            region.y = value.y;
            region.w = value.w;
            region.h = value.h;
            region.d = 1;
            // A whole texture is cycled: one that a draw already written
            // is waiting on keeps what that draw was given.
            SDL_UploadToGPUTexture(pass, &source, &region, value.whole);
            if (value.owned)
            {
                SDL_ReleaseGPUTransferBuffer(device, value.transfer);
            }
        }

        void Texture::discard(SDL_GPUDevice* device, const Upload& value)
        {
            if (value.transfer && value.owned)
            {
                SDL_ReleaseGPUTransferBuffer(device, value.transfer);
            }
        }

        void Texture::_copy(const Upload& value)
        {
            if (!value.transfer)
                return;
            // A command buffer of its own, submitted now.
            SDL_GPUDevice* device = _p->system->getDevice();
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUCopyPass* pass = SDL_BeginGPUCopyPass(cmd);
            send(device, pass, value);
            SDL_EndGPUCopyPass(pass);
            SDL_SubmitGPUCommandBuffer(cmd);
        }

        Texture::Upload Texture::_prepare(const uint8_t* data, const ImageInfo& info, int x, int y, bool whole)
        {
            FTK_P();
            Upload out;
            if (!info.isValid() || getFormat(info.type) != getFormat(p.info.type))
            {
                return out;
            }
            SDL_GPUDevice* device = p.system->getDevice();

            // The rows are packed as they go: an image's rows may be padded
            // to an alignment, and a three channel image is given a fourth.
            const int w = info.size.w;
            const int h = info.size.h;
            const int channels = getChannelCount(info.type);
            const size_t channelBytes = getChannelByteCount(info.type);
            const size_t srcPixel = channels * channelBytes;
            const size_t srcRow = info.getByteCount() / h;
            const size_t dstPixel =
                (3 == channels ? 4 : channels) *
                (p.halfFloat ? sizeof(uint16_t) : channelBytes);
            const size_t dstRow = w * dstPixel;

            // A copy of the whole texture goes through the texture's own
            // transfer buffer, made the first time and again where a copy
            // wants a larger one; the one before is let go of, which the
            // device does once it is done with it. It is mapped cycled: a
            // copy the device has not finished with keeps its memory. Two
            // copies of the whole texture made ready in one pass, with
            // nothing drawn between, would share the memory; a renderer
            // draws with a texture as it is made ready, and so ends the
            // pass before the next.
            //
            // A copy of a part, as the glyph atlas makes several of in a
            // pass, has a buffer of its own, let go of once it is sent.
            const size_t byteCount = dstRow * h;
            SDL_GPUTransferBuffer* transfer = nullptr;
            if (whole)
            {
                if (!p.transfer || byteCount > p.transferByteCount)
                {
                    if (p.transfer)
                    {
                        SDL_ReleaseGPUTransferBuffer(device, p.transfer);
                        p.transfer = nullptr;
                    }
                    SDL_GPUTransferBufferCreateInfo transferInfo = {};
                    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
                    transferInfo.size = static_cast<Uint32>(byteCount);
                    p.transfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
                    if (!p.transfer)
                    {
                        throw std::runtime_error(Format("Cannot create a transfer buffer: {0}").arg(SDL_GetError()));
                    }
                    p.transferByteCount = byteCount;
                }
                transfer = p.transfer;
            }
            else
            {
                SDL_GPUTransferBufferCreateInfo transferInfo = {};
                transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
                transferInfo.size = static_cast<Uint32>(byteCount);
                transfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
                if (!transfer)
                {
                    throw std::runtime_error(Format("Cannot create a transfer buffer: {0}").arg(SDL_GetError()));
                }
                out.owned = true;
            }
            uint8_t* dst = static_cast<uint8_t*>(SDL_MapGPUTransferBuffer(device, transfer, true));
            if (p.half)
            {
                // Each value as the half float it is kept as, and one for a
                // fourth channel that was not there.
                const std::vector<uint16_t>& table = getHalfTable();
                const size_t dstChannels = 3 == channels ? 4 : channels;
                for (int row = 0; row < h; ++row)
                {
                    const uint16_t* s = reinterpret_cast<const uint16_t*>(data + row * srcRow);
                    uint16_t* d = reinterpret_cast<uint16_t*>(dst + row * dstRow);
                    for (int i = 0; i < w; ++i, s += channels, d += dstChannels)
                    {
                        for (int c = 0; c < channels; ++c)
                        {
                            d[c] = table[s[c]];
                        }
                        if (3 == channels)
                        {
                            d[3] = 0x3C00;
                        }
                    }
                }
            }
            else if (p.halfFloat)
            {
                // Each float as the half float it is kept as, and one for
                // a fourth channel that was not there.
                const size_t dstChannels = 3 == channels ? 4 : channels;
                for (int row = 0; row < h; ++row)
                {
                    const float* s = reinterpret_cast<const float*>(data + row * srcRow);
                    uint16_t* d = reinterpret_cast<uint16_t*>(dst + row * dstRow);
                    for (int i = 0; i < w; ++i, s += channels, d += dstChannels)
                    {
                        floatToHalf(s, d, channels);
                        if (3 == channels)
                        {
                            d[3] = 0x3C00;
                        }
                    }
                }
            }
            else if (3 == channels)
            {
                for (int row = 0; row < h; ++row)
                {
                    const uint8_t* s = data + row * srcRow;
                    uint8_t* d = dst + row * dstRow;
                    for (int i = 0; i < w; ++i, s += srcPixel, d += dstPixel)
                    {
                        std::memcpy(d, s, srcPixel);
                        opaque(info.type, d + srcPixel);
                    }
                }
            }
            else if (srcRow == dstRow)
            {
                std::memcpy(dst, data, dstRow * h);
            }
            else
            {
                for (int row = 0; row < h; ++row)
                {
                    std::memcpy(dst + row * dstRow, data + row * srcRow, dstRow);
                }
            }
            SDL_UnmapGPUTransferBuffer(device, transfer);

            out.transfer = transfer;
            out.texture = p.texture;
            out.x = x;
            out.y = y;
            out.w = w;
            out.h = h;
            out.whole = whole;
            return out;
        }
    }
}
