// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "SettingsWidget.h"

#include "App.h"
#include "SettingsModel.h"

#include <ftk/UI/Divider.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace objview
{
    void SettingsWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::objview::SettingsWidget", parent);

        // Create the title label and close button.
        auto titleLabel = Label::create(context, "Settings");
        titleLabel->setMarginRole(SizeRole::MarginSmall);
        titleLabel->setHStretch(Stretch::Expanding);
        auto closeButton = ToolButton::create(context);
        closeButton->setIcon("Close");

        // Create the style widgets.
        _colorStyleComboBox = ComboBox::create(context);
        _colorStyleComboBox->setItems(getColorStyleLabels());
        _colorStyleComboBox->setHStretch(Stretch::Expanding);
        _displayScales = getDisplayScales();
        std::vector<std::string> labels;
        for (auto d : _displayScales)
        {
            labels.push_back(Format("{0}").arg(d).operator std::string());
        }
        _displayScaleComboBox = ComboBox::create(context);
        _displayScaleComboBox->setItems(labels);
        _displayScaleComboBox->setHStretch(Stretch::Expanding);

        // Layout the widgets.
        _layout = VerticalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
        auto hLayout = HorizontalLayout::create(context, _layout);
        hLayout->setSpacingRole(SizeRole::None);
        hLayout->setBackgroundRole(ColorRole::Button);
        titleLabel->setParent(hLayout);
        closeButton->setParent(hLayout);
        auto formLayout = FormLayout::create(context, _layout);
        formLayout->setMarginRole(SizeRole::Margin);
        formLayout->addRow("Color style:", _colorStyleComboBox);
        formLayout->addRow("Display scale:", _displayScaleComboBox);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, _layout);
        _scrollWidget->setBorder(false);
        _scrollWidget->setVStretch(Stretch::Expanding);
        _scrollWidget->setWidget(formLayout);

        // Set the close callback.
        std::weak_ptr<App> appWeak(app);
        closeButton->setClickedCallback(
            [appWeak]
            {
                if (auto app = appWeak.lock())
                {
                    auto window = app->getSettingsModel()->getWindow();
                    window.settings = false;
                    app->getSettingsModel()->setWindow(window);
                }
            });

        // Set the style callbacks.
        _colorStyleComboBox->setIndexCallback(
            [appWeak](int index)
            {
                if (auto app = appWeak.lock())
                {
                    auto style = app->getSettingsModel()->getStyle();
                    style.colorStyle = static_cast<ColorStyle>(index);
                    app->getSettingsModel()->setStyle(style);
                }
            });
        _displayScaleComboBox->setIndexCallback(
            [this, appWeak](int index)
            {
                if (auto app = appWeak.lock())
                {
                    auto style = app->getSettingsModel()->getStyle();
                    style.displayScale = _displayScales[index];
                    app->getSettingsModel()->setStyle(style);
                }
            });

        // Observe style settings and update the widgets.
        _styleSettingsObserver = Observer<StyleSettings>::create(
            app->getSettingsModel()->observeStyle(),
            [this](const StyleSettings& value)
            {
                _colorStyleComboBox->setCurrentIndex(static_cast<int>(value.colorStyle));
                int index = -1;
                for (int i = 0; i < static_cast<int>(_displayScales.size()); ++i)
                {
                    if (value.displayScale == _displayScales[i])
                    {
                        index = i;
                        break;
                    }
                }
                _displayScaleComboBox->setCurrentIndex(index);
            });
    }

    SettingsWidget::~SettingsWidget()
    {}

    std::shared_ptr<SettingsWidget> SettingsWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<SettingsWidget>(new SettingsWidget);
        out->_init(context, app, parent);
        return out;
    }

    Size2I SettingsWidget::getSizeHint() const
    {
        return _layout->getSizeHint();
    }

    void SettingsWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _layout->setGeometry(value);
    }
}
