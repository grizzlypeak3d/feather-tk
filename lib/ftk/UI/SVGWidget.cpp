// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/SVGWidget.h>

#include <ftk/UI/LayoutUtil.h>

#include <ftk/Core/String.h>

#include <lunasvg/lunasvg.h>

#include <optional>

namespace ftk
{
    struct SvgWidget::Private
    {
        std::vector<uint8_t> svgData;
        int svgWidth = 100;
        std::shared_ptr<Image> image;
        SizeRole marginRole = SizeRole::None;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            Size2I sizeHint;
        };
        SizeData size;
    };

    void SvgWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::SvgWidget", parent);
        setHAlign(HAlign::Left);
        setVAlign(VAlign::Center);
    }

    SvgWidget::SvgWidget() :
        _p(new Private)
    {}

    SvgWidget::~SvgWidget()
    {}

    std::shared_ptr<SvgWidget> SvgWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<SvgWidget>(new SvgWidget);
        out->_init(context, parent);
        return out;
    }

    const std::vector<uint8_t>& SvgWidget::getSVGData() const
    {
        return _p->svgData;
    }

    void SvgWidget::setSVGData(const std::vector<uint8_t>& value)
    {
        FTK_P();
        if (value == p.svgData)
            return;
        p.svgData = value;
        p.image.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    int SvgWidget::getSVGWidth() const
    {
        return _p->svgWidth;
    }

    void SvgWidget::setSVGWidth(int value)
    {
        FTK_P();
        if (value == p.svgWidth)
            return;
        p.svgWidth = value;
        p.image.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole SvgWidget::getMarginRole() const
    {
        return _p->marginRole;
    }

    void SvgWidget::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }
    
    Size2I SvgWidget::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void SvgWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        bool init = false;
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            init = true;
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
        }
        if (!p.svgData.empty() && !p.image)
        {
            init = true;
            const std::string s(p.svgData.begin(), p.svgData.end());
            if (auto doc = lunasvg::Document::loadFromData(s))
            {
                const int w = p.svgWidth;
                const float aspect = doc->height() > 0 ? (doc->width() / static_cast<float>(doc->height())) : 1.F;
                const int h = p.svgWidth / aspect;
                auto bitmap = doc->renderToBitmap(w, h, 0x00000000);
                if (!bitmap.isNull())
                {
                    p.image = Image::create(w, h, ImageType::RGBA_U8);
                    for (int y = 0; y < h; ++y)
                    {
                        uint8_t* imageP = p.image->getData() + y * w * 4;
                        const uint8_t* bitmapP = bitmap.data() + (h - 1 - y) * w * 4;
                        for (int x = 0; x < w; ++x, imageP += 4, bitmapP += 4)
                        {
                            imageP[0] = bitmapP[2];
                            imageP[1] = bitmapP[1];
                            imageP[2] = bitmapP[0];
                            imageP[3] = bitmapP[3];
                        }
                    }
                }
            }
        }

        if (init)
        {
            p.size.sizeHint = Size2I();
            if (p.image)
            {
                p.size.sizeHint.w = p.image->getWidth();
                p.size.sizeHint.h = p.image->getHeight();
            }
            p.size.sizeHint = p.size.sizeHint + p.size.margin * 2;
        }
    }

    void SvgWidget::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();
        if (p.image)
        {
            const Box2I g = margin(getGeometry(), -p.size.margin);
            const Size2I& size = p.image->getSize();
            event.render->drawImage(
                p.image,
                Box2I(
                    g.x() + g.w() / 2 - size.w / 2,
                    g.y() + g.h() / 2 - size.h / 2,
                    size.w,
                    size.h));
        }
    }
}
