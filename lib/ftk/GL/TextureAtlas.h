// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Texture.h>

#include <ftk/Core/BoxPack.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Range.h>

namespace ftk
{
    namespace gl
    {
        //! \name Texture Atlas
        ///@{
        
        //! Texture atlas item.
        struct FTK_API_TYPE TextureAtlasItem
        {
            BoxPackID id = boxPackInvalidID;
            Size2I size;
            RangeF u;
            RangeF v;
        };

        //! Texture atlas.
        class FTK_API_TYPE TextureAtlas : public std::enable_shared_from_this<TextureAtlas>
        {
            FTK_NON_COPYABLE(TextureAtlas);

        protected:
            void _init(
                int size,
                ImageType,
                ImageFilter,
                int border);

            TextureAtlas();

        public:
            FTK_API ~TextureAtlas();

            //! Create a new texture atlas.
            FTK_API static std::shared_ptr<TextureAtlas> create(
                int size,
                ImageType,
                ImageFilter = ImageFilter::Linear,
                int border = 1);

            //! Get the texture atlas size.
            FTK_API int getSize() const;

            //! Get the texture atlas type.
            FTK_API ImageType getType() const;

            //! Get the texture atlas ID.
            FTK_API unsigned int getTexture() const;

            //! Get a texture atlas item.
            FTK_API bool getItem(BoxPackID, TextureAtlasItem&);

            //! Add a texture atlas item.
            FTK_API bool addItem(const std::shared_ptr<Image>&, TextureAtlasItem&);

            //! Get the percentage of the texture atlas that is in use.
            FTK_API float getPercentageUsed() const;

        private:
            void _toItem(const std::shared_ptr<BoxPackNode>&, TextureAtlasItem&);

            FTK_PRIVATE();
        };
        
        ///@}
    }
}
