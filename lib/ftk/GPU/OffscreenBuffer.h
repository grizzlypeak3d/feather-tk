// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>

#include <ftk/Core/Color.h>
#include <ftk/Core/Image.h>

struct SDL_GPUSampler;
struct SDL_GPUTexture;

namespace ftk
{
    namespace gpu
    {
        class System;

        //! What a buffer's pixels are.
        enum class FTK_GPU_API_TYPE BufferType
        {
            RGBA_U8,
            RGBA_F16,
            RGBA_F32,

            Count,
            First = RGBA_U8
        };

        //! Offscreen buffer: what a renderer draws into.
        class FTK_GPU_API_TYPE OffscreenBuffer : public std::enable_shared_from_this<OffscreenBuffer>
        {
            FTK_NON_COPYABLE(OffscreenBuffer);

        protected:
            void _init(
                const std::shared_ptr<System>&,
                const Size2I&,
                BufferType);

            OffscreenBuffer();

        public:
            FTK_GPU_API ~OffscreenBuffer();

            //! Create a new offscreen buffer.
            FTK_GPU_API static std::shared_ptr<OffscreenBuffer> create(
                const std::shared_ptr<System>&,
                const Size2I&,
                BufferType = BufferType::RGBA_U8);

            //! Get the size.
            FTK_GPU_API const Size2I& getSize() const;

            //! Get the type.
            FTK_GPU_API BufferType getType() const;

            //! Get the texture.
            FTK_GPU_API SDL_GPUTexture* getTexture() const;

            //! Get the number IRender::drawTexture() knows the texture by.
            FTK_GPU_API unsigned int getID() const;

            //! Read the buffer back. The first row is the top one, which
            //! the image says of itself.
            FTK_GPU_API std::shared_ptr<Image> read() const;

            //! Read the buffer back as eight bits a channel, whatever it
            //! holds: what a screenshot is.
            FTK_GPU_API std::shared_ptr<Image> readU8() const;

            //! Read one pixel back, counted from the top left, as it is:
            //! values past one and below zero are kept.
            FTK_GPU_API Color4F getPixel(const V2I&) const;

        private:
            FTK_PRIVATE();
        };
    }
}
