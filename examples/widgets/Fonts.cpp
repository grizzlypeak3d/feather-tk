// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Fonts.h"

#include "App.h"

#include <ftk/UI/ComboBox.h>
#include <ftk/UI/FileEdit.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/GroupBox.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Fonts::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Fonts", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create font widgets.
        auto style = app->getStyle();
        const auto fontLabels = getFontTypeLabels();
        auto groupBox = GroupBox::create(context, "Fonts", layout);
        auto formLayout = FormLayout::create(context, groupBox);
        for (const auto font : getFontTypeEnums())
        {
            _comboBoxes[font] = ComboBox::create(context);
            _comboBoxes[font]->setIndexCallback(
                [this, style, font](int index)
                {
                    auto fonts = style->getFonts();
                    fonts[font] = _fonts[index];
                    style->setFonts(fonts);
                });
            formLayout->addRow(Format("{0}:").arg(
                fontLabels[static_cast<int>(font)]), _comboBoxes[font]);
        }

        // Create add font widgets.
        groupBox = GroupBox::create(context, "Add Fonts", layout);
        auto vLayout = VerticalLayout::create(context, groupBox);
        auto fileEdit = FileEdit::create(context, vLayout);
        auto fontSystem = context->getSystem<FontSystem>();
        fileEdit->setCallback(
            [this, fontSystem](const Path& value)
            {
                fontSystem->addFont(
                    value.getBase() + value.getNum(),
                    value.get());
            });

        // Create obervers.
        _fontsObserver = ListObserver<std::string>::create(
            fontSystem->observeFonts(),
            [this, app](const std::vector<std::string>& value)
            {
                _fonts = value;
                for (const auto font : getFontTypeEnums())
                {
                    _comboBoxes[font]->setItems(value);
                }
            });

        _styleObserver = MapObserver<FontType, std::string>::create(
            style->observeFonts(),
            [this, fontSystem](const std::map<FontType, std::string>& value)
            {
                for (const auto i : value)
                {
                    const auto j = std::find(_fonts.begin(), _fonts.end(), i.second);
                    _comboBoxes[i.first]->setCurrentIndex(j != _fonts.end() ? (j - _fonts.begin()) : -1);
                }
            });
    }

    Fonts::~Fonts()
    {}

    std::shared_ptr<IWidget> Fonts::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Fonts>(new Fonts);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I Fonts::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Fonts::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
