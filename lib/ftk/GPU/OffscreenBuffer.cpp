// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/OffscreenBuffer.h>

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

        struct OffscreenBuffer::Private
        {
            std::shared_ptr<System> system;
            Size2I size;
            BufferType type = BufferType::RGBA_U8;
            SDL_GPUTexture* texture = nullptr;
            unsigned int id = 0;
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
        }

        OffscreenBuffer::OffscreenBuffer() :
            _p(new Private)
        {}

        OffscreenBuffer::~OffscreenBuffer()
        {
            FTK_P();
            if (p.system && p.texture)
            {
                p.system->removeTexture(p.id);
                SDL_ReleaseGPUTexture(p.system->getDevice(), p.texture);
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
