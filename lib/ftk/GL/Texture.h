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
        
        //! Get the OpenGL texture format.
        FTK_API unsigned int getTextureFormat(ImageType);

        //! Get the OpenGL internal texture format.
        FTK_API unsigned int getTextureInternalFormat(ImageType);

        //! Get the OpenGL texture type.
        FTK_API unsigned int getTextureType(ImageType);

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
                const TextureOptions& = TextureOptions());

            //! Get the image information.
            FTK_API const ImageInfo& getInfo() const;

            //! Get the size.
            FTK_API const Size2I& getSize() const;

            //! Get the width.
            FTK_API int getWidth() const;

            //! Get the height.
            FTK_API int getHeight() const;

            //! Get the image type.
            FTK_API ImageType getType() const;

            //! Get the OpenGL texture ID.
            FTK_API unsigned int getID() const;

            //! \name Copy
            //! Copy image data to the texture.
            ///@{

            FTK_API void copy(const std::shared_ptr<Image>&);
            FTK_API void copy(const std::shared_ptr<Image>&, int x, int y);
            FTK_API void copy(const uint8_t*, const ImageInfo&);

            ///@}

            //! Bind the texture.
            FTK_API void bind();

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}
