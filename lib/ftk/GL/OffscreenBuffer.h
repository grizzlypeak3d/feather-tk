// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Export.h>
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
        const TextureType offscreenColorDefault = TextureType::RGBA_F32;
#elif defined(FTK_API_GLES_2)
        const TextureType offscreenColorDefault = TextureType::RGBA_U8;
#endif // FTK_API_GL_4_1

        //! Offscreen buffer depth size.
        enum class FTK_GL_API_TYPE OffscreenDepth
        {
            None,
            _16,
            _24,
            _32,

            Count,
            First = None
        };
        FTK_ENUM(FTK_GL_API, OffscreenDepth);

        //! Default offscreen depth buffer.
#if defined(FTK_API_GL_4_1)
        const OffscreenDepth offscreenDepthDefault = OffscreenDepth::_24;
#elif defined(FTK_API_GLES_2)
        const OffscreenDepth offscreenDepthDefault = OffscreenDepth::_16;
#endif // FTK_API_GL_4_1

        //! Offscreen buffer stencil size.
        enum class FTK_GL_API_TYPE OffscreenStencil
        {
            None,
            _8,

            Count,
            First = None
        };
        FTK_ENUM(FTK_GL_API, OffscreenStencil);

        //! Offscreen buffer multisampling.
        enum class FTK_GL_API_TYPE OffscreenSampling
        {
            None,
            _2,
            _4,
            _8,
            _16,

            Count,
            First = None
        };
        FTK_ENUM(FTK_GL_API, OffscreenSampling);

        //! Offscreen buffer options.
        struct FTK_GL_API_TYPE OffscreenBufferOptions
        {
            ImageFilters colorFilters;
            OffscreenDepth depth = OffscreenDepth::None;
            OffscreenStencil stencil = OffscreenStencil::None;
            OffscreenSampling sampling = OffscreenSampling::None;

            FTK_GL_API bool operator == (const OffscreenBufferOptions&) const;
            FTK_GL_API bool operator != (const OffscreenBufferOptions&) const;
        };
        
        //! Offscreen buffer.
        class FTK_GL_API_TYPE OffscreenBuffer : public std::enable_shared_from_this<OffscreenBuffer>
        {
            FTK_NON_COPYABLE(OffscreenBuffer);

        protected:
            void _init(
                const TextureInfo&,
                const OffscreenBufferOptions&);

            OffscreenBuffer();

        public:
            FTK_GL_API ~OffscreenBuffer();

            //! Create a new offscreen buffer.
            FTK_GL_API static std::shared_ptr<OffscreenBuffer> create(
                const TextureInfo&,
                const OffscreenBufferOptions& = OffscreenBufferOptions());

            //! Create a new offscreen buffer.
            FTK_GL_API static std::shared_ptr<OffscreenBuffer> create(
                const Size2I&,
                TextureType = offscreenColorDefault,
                const OffscreenBufferOptions& = OffscreenBufferOptions());

            //! Get the offscreen buffer information.
            FTK_GL_API const TextureInfo& getInfo() const;

            //! Get the offscreen buffer size.
            FTK_GL_API const Size2I& getSize() const;

            //! Get the offscreen buffer width.
            FTK_GL_API int getWidth() const;

            //! Get the offscreen buffer height.
            FTK_GL_API int getHeight() const;

            //! Get the offscreen color buffer type.
            FTK_GL_API TextureType getType() const;

            //! Get the offscreen buffer options.
            FTK_GL_API const OffscreenBufferOptions& getOptions() const;

            //! Get the offscreen buffer ID.
            FTK_GL_API unsigned int getID() const;

            //! Get the color texture ID.
            FTK_GL_API unsigned int getColorID() const;

            //! Bind the offscreen buffer.
            FTK_GL_API void bind();

            //! Get the number of objects currenty instantiated.
            FTK_GL_API static size_t getObjectCount();

            //! Get the total number of bytes currently used.
            FTK_GL_API static size_t getTotalByteCount();

        private:
            FTK_PRIVATE();
        };

        //! Check whether the offscreen buffer should be created or re-created.
        FTK_GL_API bool doCreate(
            const std::shared_ptr<OffscreenBuffer>&,
            const TextureInfo&,
            const OffscreenBufferOptions & = OffscreenBufferOptions());

        //! Check whether the offscreen buffer should be created or re-created.
        FTK_GL_API bool doCreate(
            const std::shared_ptr<OffscreenBuffer>&,
            const Size2I&,
            TextureType = offscreenColorDefault,
            const OffscreenBufferOptions & = OffscreenBufferOptions());

        //! Offscreen buffer binding.
        class FTK_GL_API_TYPE OffscreenBufferBinding
        {
        public:
            FTK_GL_API explicit OffscreenBufferBinding(const std::shared_ptr<OffscreenBuffer>&);

            FTK_GL_API ~OffscreenBufferBinding();

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}
