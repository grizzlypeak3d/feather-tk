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
        
        //! Default offscreen color buffer.
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

        //! Default offscreen depth buffer.
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
                ImageType,
                const OffscreenBufferOptions&);

            OffscreenBuffer();

        public:
            FTK_API ~OffscreenBuffer();

            //! Create a new offscreen buffer.
            FTK_API static std::shared_ptr<OffscreenBuffer> create(
                const Size2I&,
                ImageType = offscreenColorDefault,
                const OffscreenBufferOptions& = OffscreenBufferOptions());

            //! Get the offscreen buffer size.
            FTK_API const Size2I& getSize() const;

            //! Get the offscreen buffer width.
            FTK_API int getWidth() const;

            //! Get the offscreen buffer height.
            FTK_API int getHeight() const;

            //! Get the offscreen color buffer type.
            FTK_API ImageType getType() const;

            //! Get the offscreen buffer options.
            FTK_API const OffscreenBufferOptions& getOptions() const;

            //! Get the offscreen buffer ID.
            FTK_API unsigned int getID() const;

            //! Get the color texture ID.
            FTK_API unsigned int getColorID() const;

            //! Bind the offscreen buffer.
            FTK_API void bind();

            //! Get the number of objects currenty instantiated.
            FTK_API static size_t getObjectCount();

        private:
            FTK_PRIVATE();
        };

        //! Check whether the offscreen buffer should be created or re-created.
        FTK_API bool doCreate(
            const std::shared_ptr<OffscreenBuffer>&,
            const Size2I&,
            ImageType = offscreenColorDefault,
            const OffscreenBufferOptions& = OffscreenBufferOptions());

        //! Offscreen buffer binding.
        class FTK_API_TYPE OffscreenBufferBinding
        {
        public:
            FTK_API explicit OffscreenBufferBinding(const std::shared_ptr<OffscreenBuffer>&);

            FTK_API ~OffscreenBufferBinding();

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}
