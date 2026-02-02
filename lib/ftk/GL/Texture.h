// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/RenderOptions.h>

namespace ftk
{
    namespace gl
    {
        //! \name Textures
        ///@{

        //! OpenGL texture types.
        enum class TextureType
        {
            None,

            L_U8,
            L_U16,
            L_U32,
            L_F16,
            L_F32,

            LA_U8,
            LA_U16,
            LA_U32,
            LA_F16,
            LA_F32,

            RGB_U8,
            RGB_U10,
            RGB_U16,
            RGB_U32,
            RGB_F16,
            RGB_F32,

            RGBA_U8,
            RGBA_U16,
            RGBA_U32,
            RGBA_F16,
            RGBA_F32,

            Count,
            First = L_U8
        };
        FTK_ENUM(TextureType);

        //! Get a texture type for the given image type.
        FTK_API TextureType getTextureType(ImageType);

        //! Get the byte count for the given texture.
        FTK_API size_t getByteCount(const Size2I&, TextureType);
        
        //! Get the OpenGL texture format.
        FTK_API unsigned int getTextureFormat(TextureType);

        //! Get the OpenGL internal texture format.
        FTK_API unsigned int getTextureInternalFormat(TextureType);

        //! Get the OpenGL texture type.
        FTK_API unsigned int getTextureType(TextureType);

        //! Get the texture filter.
        FTK_API unsigned int getTextureFilter(ImageFilter);

        //! Texture options.
        struct FTK_API_TYPE TextureOptions
        {
            ImageFilters filters;
            bool pbo = false;

            bool operator == (const TextureOptions&) const;
            bool operator != (const TextureOptions&) const;
        };

        //! Texture.
        class FTK_API_TYPE Texture : public std::enable_shared_from_this<Texture>
        {
            FTK_NON_COPYABLE(Texture);

        protected:
            Texture(
                const ImageInfo&,
                const TextureOptions&);

        public:
            FTK_API ~Texture();

            //! Create a new texture.
            FTK_API static std::shared_ptr<Texture> create(
                const ImageInfo&,
                const TextureOptions & = TextureOptions());

            //! Get the image information.
            FTK_API const ImageInfo& getInfo() const;

            //! Get the size.
            FTK_API const Size2I& getSize() const;

            //! Get the width.
            FTK_API int getWidth() const;

            //! Get the height.
            FTK_API int getHeight() const;

            //! Get the texture type.
            FTK_API TextureType getType() const;

            //! Get the OpenGL texture ID.
            FTK_API unsigned int getID() const;

            //! \name Copy
            //! Copy image data to the texture.
            ///@{

            FTK_API bool copy(const std::shared_ptr<Image>&);
            FTK_API bool copy(const std::shared_ptr<Image>&, int x, int y);
            FTK_API bool copy(const uint8_t*, const ImageInfo&);

            ///@}

            //! Bind the texture.
            FTK_API void bind();

            //! Get the number of objects currenty instantiated.
            FTK_API static size_t getObjectCount();

            //! Get the total number of bytes currently used.
            FTK_API static size_t getTotalByteCount();

        private:
            bool _isCompatible(const ImageInfo&) const;

            FTK_PRIVATE();
        };
        
        ///@}
    }
}
