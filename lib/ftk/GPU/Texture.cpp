// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/Texture.h>

#include <ftk/GPU/System.h>

#include <ftk/Core/Format.h>

#include <SDL3/SDL.h>

#include <cstring>
#include <stdexcept>

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

            const SDL_GPUTextureFormat format = getFormat(info.type);
            if (SDL_GPU_TEXTUREFORMAT_INVALID == format || !info.isValid())
            {
                throw std::runtime_error(Format("Cannot create a texture: {0}").arg(getLabel(info)));
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
        }

        Texture::Texture() :
            _p(new Private)
        {}

        Texture::~Texture()
        {
            FTK_P();
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
            _copy(image->getData(), image->getInfo(), 0, 0, true);
        }

        void Texture::copy(const uint8_t* data, const ImageInfo& info)
        {
            _copy(data, info, 0, 0, true);
        }

        void Texture::copy(const std::shared_ptr<Image>& image, int x, int y)
        {
            _copy(image->getData(), image->getInfo(), x, y, false);
        }

        void Texture::_copy(const uint8_t* data, const ImageInfo& info, int x, int y, bool cycle)
        {
            FTK_P();
            if (!info.isValid() || getFormat(info.type) != getFormat(p.info.type))
            {
                return;
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
            const size_t dstPixel = (3 == channels ? 4 : channels) * channelBytes;
            const size_t dstRow = w * dstPixel;

            SDL_GPUTransferBufferCreateInfo transferInfo = {};
            transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            transferInfo.size = static_cast<Uint32>(dstRow * h);
            SDL_GPUTransferBuffer* transfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
            if (!transfer)
            {
                throw std::runtime_error(Format("Cannot create a transfer buffer: {0}").arg(SDL_GetError()));
            }
            uint8_t* dst = static_cast<uint8_t*>(SDL_MapGPUTransferBuffer(device, transfer, false));
            if (3 == channels)
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

            // A command buffer of its own, submitted now: what a renderer
            // is drawing is submitted when it ends, so this arrives first.
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUCopyPass* pass = SDL_BeginGPUCopyPass(cmd);
            SDL_GPUTextureTransferInfo source = {};
            source.transfer_buffer = transfer;
            SDL_GPUTextureRegion region = {};
            region.texture = p.texture;
            region.x = x;
            region.y = y;
            region.w = w;
            region.h = h;
            region.d = 1;
            SDL_UploadToGPUTexture(pass, &source, &region, cycle);
            SDL_EndGPUCopyPass(pass);
            SDL_SubmitGPUCommandBuffer(cmd);
            SDL_ReleaseGPUTransferBuffer(device, transfer);
        }
    }
}
