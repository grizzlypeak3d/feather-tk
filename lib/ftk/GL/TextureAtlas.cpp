// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/TextureAtlas.h>

#include <ftk/GL/Texture.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/BoxPack.h>

#include <map>

namespace ftk
{
    namespace gl
    {
        struct TextureAtlas::Private
        {
            int size = 0;
            ImageType type = ImageType::None;
            int border = 0;
            std::shared_ptr<Texture> texture;
            std::shared_ptr<BoxPack> boxPack;
        };

        void TextureAtlas::_init(
            int size,
            ImageType type,
            ImageFilter filter,
            int border)
        {
            FTK_P();

            p.size = size;
            p.type = type;
            p.border = border;
            TextureOptions textureOptions;
            textureOptions.filters.minify = filter;
            textureOptions.filters.magnify = filter;
            p.texture = Texture::create(ImageInfo(size, size, type), textureOptions);
            p.boxPack = BoxPack::create(Size2I(size, size), border);
        }

        TextureAtlas::TextureAtlas() :
            _p(new Private)
        {}

        TextureAtlas::~TextureAtlas()
        {}

        std::shared_ptr<TextureAtlas> TextureAtlas::create(
            int textureSize,
            ImageType textureType,
            ImageFilter filter,
            int border)
        {
            auto out = std::shared_ptr<TextureAtlas>(new TextureAtlas);
            out->_init(textureSize, textureType, filter, border);
            return out;
        }

        int TextureAtlas::getSize() const
        {
            return _p->size;
        }

        ImageType TextureAtlas::getType() const
        {
            return _p->type;
        }

        unsigned int TextureAtlas::getTexture() const
        {
            return _p->texture->getID();
        }

        bool TextureAtlas::getItem(BoxPackID id, TextureAtlasItem& item)
        {
            FTK_P();
            bool out = false;
            if (auto node = p.boxPack->getNode(id))
            {
                _toItem(node, item);
                out = true;
            }
            return out;
        }

        bool TextureAtlas::addItem(
            const std::shared_ptr<Image>& image,
            TextureAtlasItem& item)
        {
            FTK_P();
            bool out = false;
            if (auto node = p.boxPack->insert(image->getSize() + p.border * 2))
            {
                out = true;

                auto zero = Image::create(
                    node->box.size(),
                    p.type);
                zero->zero();
                p.texture->copy(
                    zero,
                    node->box.min.x,
                    node->box.min.y);

                // The glyphs are L_U8, and some GLES 2 targets want an
                // RGBA atlas. The driver used to convert on upload, which
                // WebGL refuses: an upload must match the texture. The text
                // shader reads the red channel, so the value goes in every
                // channel.
                auto upload = image;
                if (ImageType::L_U8 == image->getInfo().type &&
                    ImageType::RGBA_U8 == p.type)
                {
                    upload = Image::create(image->getSize(), p.type);
                    const uint8_t* inP = image->getData();
                    uint8_t* outP = upload->getData();
                    const size_t count =
                        image->getSize().w * image->getSize().h;
                    for (size_t i = 0; i < count; ++i, ++inP, outP += 4)
                    {
                        outP[0] = *inP;
                        outP[1] = *inP;
                        outP[2] = *inP;
                        outP[3] = *inP;
                    }
                }
                p.texture->copy(
                    upload,
                    node->box.min.x + p.border,
                    node->box.min.y + p.border);

                _toItem(node, item);
            }
            return out;
        }

        float TextureAtlas::getPercentageUsed() const
        {
            FTK_P();
            float area = 0.F;
            for (const auto& node : p.boxPack->getNodes())
            {
                if (node->id != boxPackInvalidID)
                {
                    area += ftk::area(node->box.size());
                }
            }
            return area / static_cast<float>(p.size * p.size);
        }

        void TextureAtlas::_toItem(
            const std::shared_ptr<BoxPackNode>& node,
            TextureAtlasItem& out)
        {
            FTK_P();
            out.id = node->id;
            out.u = RangeF(
                (node->box.min.x + p.border) / static_cast<float>(p.size),
                (node->box.max.x - 1 - p.border) / static_cast<float>(p.size));
            out.v = RangeF(
                (node->box.min.y + p.border) / static_cast<float>(p.size),
                (node->box.max.y - 1 - p.border) / static_cast<float>(p.size));
        }
    }
}
