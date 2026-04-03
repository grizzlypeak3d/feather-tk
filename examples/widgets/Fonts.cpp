// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Fonts.h"

#include "App.h"

#include <ftk/UI/ComboBox.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>

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
        layout->setMarginRole(SizeRole::MarginLarge);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create font widgets.
        auto fontSystem = context->getSystem<FontSystem>();
        const auto fontNames = fontSystem->getFontNames();
        auto style = app->getStyle();
        auto fontRoles = style->getFontRoles();

        auto regularComboBox = ComboBox::create(context, fontNames);
        auto i = std::find(fontNames.begin(), fontNames.end(), fontRoles[FontRole::Label].name);
        if (i != fontNames.end())
        {
            regularComboBox->setCurrentIndex(i - fontNames.begin());
        }
        auto formLayout = FormLayout::create(context, layout);
        formLayout->addRow("Regular:", regularComboBox);

        auto titleComboBox = ComboBox::create(context, fontNames);
        i = std::find(fontNames.begin(), fontNames.end(), fontRoles[FontRole::Title].name);
        if (i != fontNames.end())
        {
            titleComboBox->setCurrentIndex(i - fontNames.begin());
        }
        formLayout->addRow("Title:", titleComboBox);

        auto monoComboBox = ComboBox::create(context, fontNames);
        i = std::find(fontNames.begin(), fontNames.end(), fontRoles[FontRole::Mono].name);
        if (i != fontNames.end())
        {
            monoComboBox->setCurrentIndex(i - fontNames.begin());
        }
        formLayout->addRow("Mono:", monoComboBox);

        auto symbolsComboBox = ComboBox::create(context, fontNames);
        i = std::find(fontNames.begin(), fontNames.end(), fontRoles[FontRole::Symbols].name);
        if (i != fontNames.end())
        {
            symbolsComboBox->setCurrentIndex(i - fontNames.begin());
        }
        formLayout->addRow("Symbols:", symbolsComboBox);

        regularComboBox->setIndexCallback(
            [style, fontNames](int index)
            {
                auto fontRoles = style->getFontRoles();
                fontRoles[FontRole::Label].name = fontNames[index];
                style->setFontRoles(fontRoles);
            });

        titleComboBox->setIndexCallback(
            [app, fontNames](int index)
            {
            });

        monoComboBox->setIndexCallback(
            [app, fontNames](int index)
            {
            });

        symbolsComboBox->setIndexCallback(
            [app, fontNames](int index)
            {
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
