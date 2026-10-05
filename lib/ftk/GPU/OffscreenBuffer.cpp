// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/OffscreenBuffer.h>

#include <ftk/GPU/System.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Memory.h>

#include <SDL3/SDL.h>

#include <atomic>
#include <cstring>
#include <stdexcept>

namespace ftk
{
    namespace gpu
    {
        namespace
        {
            SDL_GPUTextureFormat getFormat(BufferType value)
            {
                SDL_GPUTextureFormat out = SDL_GPU_TEXTUREFORMAT_INVALID;
                switch (value)
                {
                case BufferType::RGBA_U8: out = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM; break;
                case BufferType::RGBA_F16: out = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT; break;
                case BufferType::RGBA_F32: out = SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT; break;
                default: break;
                }
                return out;
            }

            ImageType getImageType(BufferType value)
            {
                ImageType out = ImageType::None;
                switch (value)
                {
                case BufferType::RGBA_U8: out = ImageType::RGBA_U8; break;
                case BufferType::RGBA_F16: out = ImageType::RGBA_F16; break;
                case BufferType::RGBA_F32: out = ImageType::RGBA_F32; break;
                default: break;
                }
                return out;
            }
        }

        namespace
        {
            std::atomic<size_t> objectCount = 0;
            std::atomic<size_t> totalByteCount = 0;

            size_t getByteCount(const Size2I& size, BufferType type)
            {
                return ImageInfo(size, getImageType(type)).getByteCount();
            }
        }

        struct OffscreenBuffer::Private
        {
            std::shared_ptr<System> system;
            Size2I size;
            BufferType type = BufferType::RGBA_U8;
            SDL_GPUTexture* texture = nullptr;
            unsigned int id = 0;

            // What read(const ImageInfo&) converts into and reads back
            // through, kept from one read to the next: an export reads
            // every frame the same way, and making these each time was a
            // good part of what a read cost.
            SDL_GPUTexture* readTexture = nullptr;
            SDL_GPUTextureFormat readTextureFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
            SDL_GPUTransferBuffer* readTransfer = nullptr;
            size_t readTransferByteCount = 0;
        };

        void OffscreenBuffer::_init(
            const std::shared_ptr<System>& system,
            const Size2I& size,
            BufferType type)
        {
            FTK_P();
            p.system = system;
            p.size = size;
            p.type = type;
            if (!size.isValid())
            {
                throw std::runtime_error("Invalid offscreen buffer");
            }
            SDL_GPUTextureCreateInfo info = {};
            info.type = SDL_GPU_TEXTURETYPE_2D;
            info.format = getFormat(type);
            info.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
            info.width = size.w;
            info.height = size.h;
            info.layer_count_or_depth = 1;
            info.num_levels = 1;
            p.texture = SDL_CreateGPUTexture(system->getDevice(), &info);
            if (!p.texture)
            {
                throw std::runtime_error(Format("Cannot create an offscreen buffer: {0}").arg(SDL_GetError()));
            }
            p.id = system->addTexture(p.texture);
            ++objectCount;
            totalByteCount += getByteCount(p.size, p.type);
        }

        OffscreenBuffer::OffscreenBuffer() :
            _p(new Private)
        {}

        OffscreenBuffer::~OffscreenBuffer()
        {
            FTK_P();
            if (p.system && p.texture)
            {
                --objectCount;
                totalByteCount -= getByteCount(p.size, p.type);
                p.system->removeTexture(p.id);
                SDL_ReleaseGPUTexture(p.system->getDevice(), p.texture);
            }
            if (p.system && p.readTexture)
            {
                SDL_ReleaseGPUTexture(p.system->getDevice(), p.readTexture);
            }
            if (p.system && p.readTransfer)
            {
                SDL_ReleaseGPUTransferBuffer(p.system->getDevice(), p.readTransfer);
            }
        }

        std::shared_ptr<OffscreenBuffer> OffscreenBuffer::create(
            const std::shared_ptr<System>& system,
            const Size2I& size,
            BufferType type)
        {
            auto out = std::shared_ptr<OffscreenBuffer>(new OffscreenBuffer);
            out->_init(system, size, type);
            return out;
        }

        size_t OffscreenBuffer::getObjectCount()
        {
            return objectCount;
        }

        size_t OffscreenBuffer::getTotalByteCount()
        {
            return totalByteCount;
        }

        const Size2I& OffscreenBuffer::getSize() const
        {
            return _p->size;
        }

        BufferType OffscreenBuffer::getType() const
        {
            return _p->type;
        }

        SDL_GPUTexture* OffscreenBuffer::getTexture() const
        {
            return _p->texture;
        }

        unsigned int OffscreenBuffer::getID() const
        {
            return _p->id;
        }

        namespace
        {
            std::shared_ptr<Image> download(
                SDL_GPUDevice*,
                SDL_GPUCommandBuffer*,
                SDL_GPUTexture*,
                const Size2I&,
                ImageType);
        }

        std::shared_ptr<Image> OffscreenBuffer::read() const
        {
            FTK_P();
            SDL_GPUDevice* device = p.system->getDevice();
            return download(
                device,
                SDL_AcquireGPUCommandBuffer(device),
                p.texture,
                p.size,
                getImageType(p.type));
        }

        std::shared_ptr<Image> OffscreenBuffer::readU8() const
        {
            FTK_P();
            if (BufferType::RGBA_U8 == p.type)
            {
                return read();
            }
            // Converted where it is, by drawing it into eight bits.
            SDL_GPUDevice* device = p.system->getDevice();
            SDL_GPUTextureCreateInfo info = {};
            info.type = SDL_GPU_TEXTURETYPE_2D;
            info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
            info.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
            info.width = p.size.w;
            info.height = p.size.h;
            info.layer_count_or_depth = 1;
            info.num_levels = 1;
            SDL_GPUTexture* tmp = SDL_CreateGPUTexture(device, &info);
            if (!tmp)
            {
                throw std::runtime_error(Format("Cannot create a texture: {0}").arg(SDL_GetError()));
            }
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUBlitInfo blit = {};
            blit.source.texture = p.texture;
            blit.source.w = p.size.w;
            blit.source.h = p.size.h;
            blit.destination.texture = tmp;
            blit.destination.w = p.size.w;
            blit.destination.h = p.size.h;
            blit.load_op = SDL_GPU_LOADOP_DONT_CARE;
            blit.filter = SDL_GPU_FILTER_NEAREST;
            SDL_BlitGPUTexture(cmd, &blit);
            auto out = download(device, cmd, tmp, p.size, ImageType::RGBA_U8);
            SDL_ReleaseGPUTexture(device, tmp);
            return out;
        }

        namespace
        {
            // What a type of image is read back through: a texture format
            // with the same components, of one channel or of four, since
            // there are none of three.
            struct ReadFormat
            {
                SDL_GPUTextureFormat format = SDL_GPU_TEXTUREFORMAT_INVALID;
                ImageType type = ImageType::None;
            };

            ReadFormat getReadFormat(ImageType value)
            {
                ReadFormat out;
                switch (value)
                {
                case ImageType::L_U8:
                    out = { SDL_GPU_TEXTUREFORMAT_R8_UNORM, ImageType::L_U8 };
                    break;
                case ImageType::L_U16:
                    out = { SDL_GPU_TEXTUREFORMAT_R16_UNORM, ImageType::L_U16 };
                    break;
                case ImageType::L_F16:
                    out = { SDL_GPU_TEXTUREFORMAT_R16_FLOAT, ImageType::L_F16 };
                    break;
                case ImageType::L_F32:
                    out = { SDL_GPU_TEXTUREFORMAT_R32_FLOAT, ImageType::L_F32 };
                    break;
                case ImageType::RGB_U8:
                case ImageType::RGBA_U8:
                    out = { SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM, ImageType::RGBA_U8 };
                    break;
                case ImageType::RGB_U10:
                case ImageType::RGB_U16:
                case ImageType::RGBA_U16:
                    out = { SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM, ImageType::RGBA_U16 };
                    break;
                case ImageType::RGB_F16:
                case ImageType::RGBA_F16:
                    out = { SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT, ImageType::RGBA_F16 };
                    break;
                case ImageType::RGB_F32:
                case ImageType::RGBA_F32:
                    out = { SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT, ImageType::RGBA_F32 };
                    break;
                default: break;
                }
                return out;
            }
        }

        bool OffscreenBuffer::canRead(ImageType value)
        {
            return getReadFormat(value).format != SDL_GPU_TEXTUREFORMAT_INVALID;
        }

        std::shared_ptr<Image> OffscreenBuffer::read(const ImageInfo& info) const
        {
            FTK_P();
            const ReadFormat readFormat = getReadFormat(info.type);
            if (SDL_GPU_TEXTUREFORMAT_INVALID == readFormat.format || info.size != p.size)
            {
                throw std::runtime_error(Format("Cannot read the buffer as: {0}").arg(getLabel(info)));
            }

            // Converted where it is, by drawing it into the components
            // wanted, unless it holds them already.
            SDL_GPUDevice* device = p.system->getDevice();
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUTexture* texture = p.texture;
            if (getFormat(p.type) != readFormat.format)
            {
                if (p.readTexture && p.readTextureFormat != readFormat.format)
                {
                    SDL_ReleaseGPUTexture(device, p.readTexture);
                    p.readTexture = nullptr;
                }
                if (!p.readTexture)
                {
                    SDL_GPUTextureCreateInfo textureInfo = {};
                    textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
                    textureInfo.format = readFormat.format;
                    textureInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
                    textureInfo.width = p.size.w;
                    textureInfo.height = p.size.h;
                    textureInfo.layer_count_or_depth = 1;
                    textureInfo.num_levels = 1;
                    p.readTexture = SDL_GPUTextureSupportsFormat(
                        device,
                        textureInfo.format,
                        textureInfo.type,
                        textureInfo.usage) ?
                        SDL_CreateGPUTexture(device, &textureInfo) :
                        nullptr;
                    p.readTextureFormat = readFormat.format;
                }
                if (!p.readTexture)
                {
                    SDL_CancelGPUCommandBuffer(cmd);
                    throw std::runtime_error(Format("Cannot read the buffer as: {0}").arg(getLabel(info)));
                }
                SDL_GPUBlitInfo blit = {};
                blit.source.texture = p.texture;
                blit.source.w = p.size.w;
                blit.source.h = p.size.h;
                blit.destination.texture = p.readTexture;
                blit.destination.w = p.size.w;
                blit.destination.h = p.size.h;
                blit.load_op = SDL_GPU_LOADOP_DONT_CARE;
                blit.filter = SDL_GPU_FILTER_NEAREST;
                SDL_BlitGPUTexture(cmd, &blit);
                texture = p.readTexture;
            }

            const size_t w = static_cast<size_t>(p.size.w);
            const size_t h = static_cast<size_t>(p.size.h);
            const size_t componentSize = static_cast<size_t>(getBitDepth(readFormat.type)) / 8;
            const size_t readChannels = static_cast<size_t>(getChannelCount(readFormat.type));
            const size_t readStride = w * readChannels * componentSize;
            const size_t readByteCount = readStride * h;
            if (p.readTransfer && p.readTransferByteCount != readByteCount)
            {
                SDL_ReleaseGPUTransferBuffer(device, p.readTransfer);
                p.readTransfer = nullptr;
            }
            if (!p.readTransfer)
            {
                SDL_GPUTransferBufferCreateInfo transferInfo = {};
                transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
                transferInfo.size = static_cast<Uint32>(readByteCount);
                p.readTransfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
                p.readTransferByteCount = readByteCount;
            }
            if (!p.readTransfer)
            {
                SDL_CancelGPUCommandBuffer(cmd);
                throw std::runtime_error(Format("Cannot read the buffer as: {0}").arg(getLabel(info)));
            }
            SDL_GPUCopyPass* pass = SDL_BeginGPUCopyPass(cmd);
            SDL_GPUTextureRegion region = {};
            region.texture = texture;
            region.w = p.size.w;
            region.h = p.size.h;
            region.d = 1;
            SDL_GPUTextureTransferInfo destination = {};
            destination.transfer_buffer = p.readTransfer;
            SDL_DownloadFromGPUTexture(pass, &region, &destination);
            SDL_EndGPUCopyPass(pass);
            SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
            SDL_WaitForGPUFences(device, true, &fence, 1);
            SDL_ReleaseGPUFence(device, fence);

            // Into the layout asked for, straight from what was read back,
            // which has its top row first, rows that are not padded, and
            // this machine's byte order.
            auto out = Image::create(info);
            const uint8_t* read = static_cast<const uint8_t*>(
                SDL_MapGPUTransferBuffer(device, p.readTransfer, false));
            const size_t outStride = out->getByteCount() / h;
            const bool swap = info.layout.endian != getEndian();
            const bool same = getChannelCount(info.type) == static_cast<int>(readChannels);
            for (size_t y = 0; y < h; ++y)
            {
                const uint8_t* in = read + y * readStride;
                uint8_t* o = out->getData() + (info.layout.mirror.y ? y : (h - 1 - y)) * outStride;
                size_t swapSize = componentSize;
                size_t swapCount = 0;
                if (ImageType::RGB_U10 == info.type)
                {
                    // Ten bits of each in a word, red highest, as OpenGL's
                    // GL_UNSIGNED_INT_10_10_10_2 has them.
                    const uint16_t* in16 = reinterpret_cast<const uint16_t*>(in);
                    uint32_t* o32 = reinterpret_cast<uint32_t*>(o);
                    for (size_t x = 0; x < w; ++x, in16 += 4)
                    {
                        o32[x] =
                            (static_cast<uint32_t>(in16[0] >> 6) << 22) |
                            (static_cast<uint32_t>(in16[1] >> 6) << 12) |
                            (static_cast<uint32_t>(in16[2] >> 6) << 2) |
                            static_cast<uint32_t>(in16[3] >> 14);
                    }
                    swapSize = 4;
                    swapCount = w;
                }
                else if (same)
                {
                    std::memcpy(o, in, readStride);
                    swapCount = w * readChannels;
                }
                else
                {
                    // Three channels of the four, a component at a time
                    // for the sizes there are: a copy of each pixel was
                    // most of what a frame took to lay out.
                    switch (componentSize)
                    {
                    case 1:
                        for (size_t x = 0; x < w; ++x, in += 4, o += 3)
                        {
                            o[0] = in[0];
                            o[1] = in[1];
                            o[2] = in[2];
                        }
                        o -= w * 3;
                        break;
                    case 2:
                    {
                        const uint16_t* in16 = reinterpret_cast<const uint16_t*>(in);
                        uint16_t* o16 = reinterpret_cast<uint16_t*>(o);
                        for (size_t x = 0; x < w; ++x, in16 += 4, o16 += 3)
                        {
                            o16[0] = in16[0];
                            o16[1] = in16[1];
                            o16[2] = in16[2];
                        }
                        break;
                    }
                    default:
                    {
                        const uint32_t* in32 = reinterpret_cast<const uint32_t*>(in);
                        uint32_t* o32 = reinterpret_cast<uint32_t*>(o);
                        for (size_t x = 0; x < w; ++x, in32 += 4, o32 += 3)
                        {
                            o32[0] = in32[0];
                            o32[1] = in32[1];
                            o32[2] = in32[2];
                        }
                        break;
                    }
                    }
                    swapCount = w * 3;
                }
                if (swap && swapSize > 1)
                {
                    swapEndian(o, swapCount, swapSize);
                }
            }
            SDL_UnmapGPUTransferBuffer(device, p.readTransfer);
            return out;
        }

        Color4F OffscreenBuffer::getPixel(const V2I& pos) const
        {
            FTK_P();
            Color4F out;
            if (pos.x < 0 || pos.y < 0 || pos.x >= p.size.w || pos.y >= p.size.h)
            {
                return out;
            }
            // The one pixel, drawn into a float texture of its own: nothing
            // else has to be read back, and what comes back is floats
            // whatever the buffer holds.
            SDL_GPUDevice* device = p.system->getDevice();
            SDL_GPUTextureCreateInfo info = {};
            info.type = SDL_GPU_TEXTURETYPE_2D;
            info.format = SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT;
            info.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
            info.width = 1;
            info.height = 1;
            info.layer_count_or_depth = 1;
            info.num_levels = 1;
            SDL_GPUTexture* tmp = SDL_CreateGPUTexture(device, &info);
            if (!tmp)
            {
                return out;
            }
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUBlitInfo blit = {};
            blit.source.texture = p.texture;
            blit.source.x = pos.x;
            blit.source.y = pos.y;
            blit.source.w = 1;
            blit.source.h = 1;
            blit.destination.texture = tmp;
            blit.destination.w = 1;
            blit.destination.h = 1;
            blit.load_op = SDL_GPU_LOADOP_DONT_CARE;
            blit.filter = SDL_GPU_FILTER_NEAREST;
            SDL_BlitGPUTexture(cmd, &blit);
            const auto image = download(device, cmd, tmp, Size2I(1, 1), ImageType::RGBA_F32);
            SDL_ReleaseGPUTexture(device, tmp);
            const float* data = reinterpret_cast<const float*>(image->getData());
            out = Color4F(data[0], data[1], data[2], data[3]);
            return out;
        }

        namespace
        {
            std::shared_ptr<Image> download(
                SDL_GPUDevice* device,
                SDL_GPUCommandBuffer* cmd,
                SDL_GPUTexture* texture,
                const Size2I& size,
                ImageType type)
            {
            struct { SDL_GPUTexture* texture; Size2I size; } p = { texture, size };
            ImageInfo info(size, type);
            info.layout.mirror.y = true;
            auto out = Image::create(info);

            SDL_GPUTransferBufferCreateInfo transferInfo = {};
            transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
            transferInfo.size = static_cast<Uint32>(out->getByteCount());
            SDL_GPUTransferBuffer* transfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
            SDL_GPUCopyPass* pass = SDL_BeginGPUCopyPass(cmd);
            SDL_GPUTextureRegion region = {};
            region.texture = p.texture;
            region.w = p.size.w;
            region.h = p.size.h;
            region.d = 1;
            SDL_GPUTextureTransferInfo destination = {};
            destination.transfer_buffer = transfer;
            SDL_DownloadFromGPUTexture(pass, &region, &destination);
            SDL_EndGPUCopyPass(pass);
            // Everything submitted before this has drawn by the time the
            // fence is passed.
            SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
            SDL_WaitForGPUFences(device, true, &fence, 1);
            SDL_ReleaseGPUFence(device, fence);
            std::memcpy(
                out->getData(),
                SDL_MapGPUTransferBuffer(device, transfer, false),
                out->getByteCount());
            SDL_UnmapGPUTransferBuffer(device, transfer);
            SDL_ReleaseGPUTransferBuffer(device, transfer);
            return out;
            }
        }
    }
}
