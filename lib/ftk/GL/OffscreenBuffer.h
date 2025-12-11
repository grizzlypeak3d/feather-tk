// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Texture.h>

#include <sstream>

namespace ftk
{
    namespace gl
    {
        //! \name Offscreen Buffers
        ///@{
        
        //! Default offscreen buffer color type.
#if defined(FTK_API_GL_4_1)
        const ImageType offscreenColorDefault = ImageType::RGBA_F32;
#elif defined(FTK_API_GLES_2)
        const ImageType offscreenColorDefault = ImageType::RGBA_U8;
#endif // FTK_API_GL_4_1

        //! Offscreen buffer depth size.
        enum class FTK_API_TYPE OffscreenDepth
        {
            None,
            _16,
            _24,
            _32,

            Count,
            First = None
        };
        FTK_ENUM(OffscreenDepth);

        //! Default offscreen buffer color type.
#if defined(FTK_API_GL_4_1)
        const OffscreenDepth offscreenDepthDefault = OffscreenDepth::_16;
#elif defined(FTK_API_GLES_2)
        const OffscreenDepth offscreenDepthDefault = OffscreenDepth::_24;
#endif // FTK_API_GL_4_1

        //! Offscreen buffer stencil size.
        enum class FTK_API_TYPE OffscreenStencil
        {
            None,
            _8,

            Count,
            First = None
        };
        FTK_ENUM(OffscreenStencil);

        //! Offscreen buffer multisampling.
        enum class FTK_API_TYPE OffscreenSampling
        {
            None,
            _2,
            _4,
            _8,
            _16,

            Count,
            First = None
        };
        FTK_ENUM(OffscreenSampling);

        //! Offscreen buffer options.
        struct FTK_API_TYPE OffscreenBufferOptions
        {
            ImageType color = ImageType::None;
            ImageFilters colorFilters;
            OffscreenDepth depth = OffscreenDepth::None;
            OffscreenStencil stencil = OffscreenStencil::None;
            OffscreenSampling sampling = OffscreenSampling::None;

            bool operator == (const OffscreenBufferOptions&) const;
            bool operator != (const OffscreenBufferOptions&) const;
        };
        
        //! Offscreen buffer.
        class FTK_API_TYPE OffscreenBuffer : public std::enable_shared_from_this<OffscreenBuffer>
        {
            FTK_NON_COPYABLE(OffscreenBuffer);

        protected:
            void _init(
                const Size2I&,
                const OffscreenBufferOptions&);

            OffscreenBuffer();

        public:
            ~OffscreenBuffer();

            //! Create a new offscreen buffer.
            static std::shared_ptr<OffscreenBuffer> create(
                const Size2I&,
                const OffscreenBufferOptions&);

            //! Get the offscreen buffer size.
            const Size2I& getSize() const;

            //! Get the offscreen buffer width.
            int getWidth() const;

            //! Get the offscreen buffer height.
            int getHeight() const;

            //! Get the options.
            const OffscreenBufferOptions& getOptions() const;

            //! Get the offscreen buffer ID.
            unsigned int getID() const;

            //! Get the color texture ID.
            unsigned int getColorID() const;

            //! Bind the offscreen buffer.
            void bind();

        private:
            FTK_PRIVATE();
        };

        //! Check whether the offscreen buffer should be created or re-created.
        FTK_API bool doCreate(
            const std::shared_ptr<OffscreenBuffer>&,
            const Size2I&,
            const OffscreenBufferOptions&);

        //! Offscreen buffer binding.
        class FTK_API_TYPE OffscreenBufferBinding
        {
        public:
            explicit OffscreenBufferBinding(const std::shared_ptr<OffscreenBuffer>&);

            ~OffscreenBufferBinding();

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}
