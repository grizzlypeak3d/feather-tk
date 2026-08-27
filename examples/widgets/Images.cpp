// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Images.h"

#include <ftk/UI/ImageWidget.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/SVGWidget.h>

#include <ftk/Core/ImageIO.h>

#include <cmath>

using namespace ftk;

namespace ftk_resource
{
    extern std::vector<uint8_t> Charlie;
    extern std::vector<uint8_t> TestDrawing;
}

namespace widgets
{
    void Images::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Images", parent);

        // Create a layout: a captioned column per image.
        auto layout = HorizontalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create an image widget with an image read from a file.
        auto column = VerticalLayout::create(context, layout);
        auto imageWidget = ImageWidget::create(context, column);
        auto imageIO = context->getSystem<ImageIO>();
        MemFile memFile(nullptr, ftk_resource::Charlie.data(), ftk_resource::Charlie.size());
        if (auto read = imageIO->read("Charlie.png", memFile))
        {
            imageWidget->setImage(read->read());
        }
        auto label = Label::create(context, "PNG", column);
        label->setHAlign(HAlign::Center);

        // Create an SVG widget.
        column = VerticalLayout::create(context, layout);
        auto svgWidget = SvgWidget::create(context, column);
        svgWidget->setSVGData(ftk_resource::TestDrawing);
        svgWidget->setSVGWidth(300);
        label = Label::create(context, "SVG", column);
        label->setHAlign(HAlign::Center);

        // Create image widgets with images made in code.
        column = VerticalLayout::create(context, layout);
        const int size = 150;
        auto image = Image::create(size, size, ImageType::RGBA_U8);
        uint8_t* p = image->getData();
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                const bool checker = ((x / 25) % 2) != ((y / 25) % 2);
                p[0] = checker ? 220 : 90;
                p[1] = checker ? 160 : 90;
                p[2] = checker ? 60 : 90;
                p[3] = 255;
                p += 4;
            }
        }
        imageWidget = ImageWidget::create(context, column);
        imageWidget->setImage(image);
        label = Label::create(context, "Checkers", column);
        label->setHAlign(HAlign::Center);

        column = VerticalLayout::create(context, layout);
        image = Image::create(size, size, ImageType::RGBA_U8);
        p = image->getData();
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                const float v = std::sin(x / 8.F) * std::sin(y / 8.F) * .5F + .5F;
                p[0] = 40 + v * 180;
                p[1] = 60 + v * 120;
                p[2] = 120 + v * 135;
                p[3] = 255;
                p += 4;
            }
        }
        imageWidget = ImageWidget::create(context, column);
        imageWidget->setImage(image);
        label = Label::create(context, "Waves", column);
        label->setHAlign(HAlign::Center);
    }

    Images::~Images()
    {}

    std::shared_ptr<IWidget> Images::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Images>(new Images);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I Images::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Images::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
