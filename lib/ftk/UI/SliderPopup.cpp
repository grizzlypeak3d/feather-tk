// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/SliderPopup.h>

#include <ftk/UI/DoubleEdit.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/RowLayout.h>

#include <limits>

namespace ftk
{
    struct FloatSliderPopup::Private
    {
        std::shared_ptr<PushButton> resetButton;
        std::shared_ptr<FloatEdit> minEdit;
        std::shared_ptr<FloatEdit> maxEdit;
        std::shared_ptr<VerticalLayout> layout;
        std::shared_ptr<Observer<RangeF> > rangeObserver;
    };

    void FloatSliderPopup::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidgetPopup::_init(context, "ftk::FloatSliderPopup", parent);
        FTK_P();

        p.resetButton = PushButton::create(context, "Reset");
        p.resetButton->setTooltip(
            "Reset to the default value.\n"
            "\n"
            "Double clicking the slider also resets it.");
        p.resetButton->setEnabled(model->hasDefault());

        // The edits get their own models so the range being edited does
        // not constrain what can be typed into them.
        p.minEdit = FloatEdit::create(context);
        p.minEdit->setRange(-1e9F, 1e9F);
        p.minEdit->setStep(model->getStep());
        p.minEdit->setLargeStep(model->getLargeStep());
        p.maxEdit = FloatEdit::create(context);
        p.maxEdit->setRange(-1e9F, 1e9F);
        p.maxEdit->setStep(model->getStep());
        p.maxEdit->setLargeStep(model->getLargeStep());

        p.layout = VerticalLayout::create(context);
        p.layout->setMarginRole(SizeRole::MarginSmall);
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.resetButton->setParent(p.layout);
        // A soft range is a starting point the user may adjust, so it is
        // offered here; a hard range is a fixed bound.
        if (model->isRangeSoft())
        {
            auto formLayout = FormLayout::create(context, p.layout);
            formLayout->setSpacingRole(SizeRole::SpacingSmall);
            formLayout->addRow("Minimum:", p.minEdit);
            formLayout->addRow("Maximum:", p.maxEdit);
        }
        setWidget(p.layout);

        std::weak_ptr<FloatModel> modelResetWeak(model);
        p.resetButton->setClickedCallback(
            [modelResetWeak]
            {
                if (auto model = modelResetWeak.lock())
                {
                    model->setDefault();
                }
            });

        std::weak_ptr<FloatModel> modelWeak(model);
        p.minEdit->setCallback(
            [modelWeak](float value)
            {
                if (auto model = modelWeak.lock())
                {
                    const RangeF& range = model->getRange();
                    model->setRange(RangeF(value, std::max(value, range.max())));
                }
            });
        p.maxEdit->setCallback(
            [modelWeak](float value)
            {
                if (auto model = modelWeak.lock())
                {
                    const RangeF& range = model->getRange();
                    model->setRange(RangeF(std::min(value, range.min()), value));
                }
            });

        p.rangeObserver = Observer<RangeF>::create(
            model->observeRange(),
            [this](const RangeF& value)
            {
                FTK_P();
                p.minEdit->setValue(value.min());
                p.maxEdit->setValue(value.max());
            });
    }

    FloatSliderPopup::FloatSliderPopup() :
        _p(new Private)
    {}

    FloatSliderPopup::~FloatSliderPopup()
    {}

    std::shared_ptr<FloatSliderPopup> FloatSliderPopup::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatSliderPopup>(new FloatSliderPopup);
        out->_init(context, model, parent);
        return out;
    }

    struct DoubleSliderPopup::Private
    {
        std::shared_ptr<PushButton> resetButton;
        std::shared_ptr<DoubleEdit> minEdit;
        std::shared_ptr<DoubleEdit> maxEdit;
        std::shared_ptr<VerticalLayout> layout;
        std::shared_ptr<Observer<RangeD> > rangeObserver;
    };

    void DoubleSliderPopup::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidgetPopup::_init(context, "ftk::DoubleSliderPopup", parent);
        FTK_P();

        p.resetButton = PushButton::create(context, "Reset");
        p.resetButton->setTooltip(
            "Reset to the default value.\n"
            "\n"
            "Double clicking the slider also resets it.");
        p.resetButton->setEnabled(model->hasDefault());

        // The edits get their own models so the range being edited does
        // not constrain what can be typed into them.
        p.minEdit = DoubleEdit::create(context);
        p.minEdit->setRange(-1e9, 1e9);
        p.minEdit->setStep(model->getStep());
        p.minEdit->setLargeStep(model->getLargeStep());
        p.maxEdit = DoubleEdit::create(context);
        p.maxEdit->setRange(-1e9, 1e9);
        p.maxEdit->setStep(model->getStep());
        p.maxEdit->setLargeStep(model->getLargeStep());

        p.layout = VerticalLayout::create(context);
        p.layout->setMarginRole(SizeRole::MarginSmall);
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.resetButton->setParent(p.layout);
        // A soft range is a starting point the user may adjust, so it is
        // offered here; a hard range is a fixed bound.
        if (model->isRangeSoft())
        {
            auto formLayout = FormLayout::create(context, p.layout);
            formLayout->setSpacingRole(SizeRole::SpacingSmall);
            formLayout->addRow("Minimum:", p.minEdit);
            formLayout->addRow("Maximum:", p.maxEdit);
        }
        setWidget(p.layout);

        std::weak_ptr<DoubleModel> modelResetWeak(model);
        p.resetButton->setClickedCallback(
            [modelResetWeak]
            {
                if (auto model = modelResetWeak.lock())
                {
                    model->setDefault();
                }
            });

        std::weak_ptr<DoubleModel> modelWeak(model);
        p.minEdit->setCallback(
            [modelWeak](double value)
            {
                if (auto model = modelWeak.lock())
                {
                    const RangeD& range = model->getRange();
                    model->setRange(RangeD(value, std::max(value, range.max())));
                }
            });
        p.maxEdit->setCallback(
            [modelWeak](double value)
            {
                if (auto model = modelWeak.lock())
                {
                    const RangeD& range = model->getRange();
                    model->setRange(RangeD(std::min(value, range.min()), value));
                }
            });

        p.rangeObserver = Observer<RangeD>::create(
            model->observeRange(),
            [this](const RangeD& value)
            {
                FTK_P();
                p.minEdit->setValue(value.min());
                p.maxEdit->setValue(value.max());
            });
    }

    DoubleSliderPopup::DoubleSliderPopup() :
        _p(new Private)
    {}

    DoubleSliderPopup::~DoubleSliderPopup()
    {}

    std::shared_ptr<DoubleSliderPopup> DoubleSliderPopup::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleSliderPopup>(new DoubleSliderPopup);
        out->_init(context, model, parent);
        return out;
    }

    struct IntSliderPopup::Private
    {
        std::shared_ptr<PushButton> resetButton;
        std::shared_ptr<IntEdit> minEdit;
        std::shared_ptr<IntEdit> maxEdit;
        std::shared_ptr<VerticalLayout> layout;
        std::shared_ptr<Observer<RangeI> > rangeObserver;
    };

    void IntSliderPopup::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidgetPopup::_init(context, "ftk::IntSliderPopup", parent);
        FTK_P();

        p.resetButton = PushButton::create(context, "Reset");
        p.resetButton->setTooltip(
            "Reset to the default value.\n"
            "\n"
            "Double clicking the slider also resets it.");
        p.resetButton->setEnabled(model->hasDefault());

        // The edits get their own models so the range being edited does
        // not constrain what can be typed into them.
        p.minEdit = IntEdit::create(context);
        p.minEdit->setRange(
            std::numeric_limits<int>::lowest(),
            std::numeric_limits<int>::max());
        p.minEdit->setStep(model->getStep());
        p.minEdit->setLargeStep(model->getLargeStep());
        p.maxEdit = IntEdit::create(context);
        p.maxEdit->setRange(
            std::numeric_limits<int>::lowest(),
            std::numeric_limits<int>::max());
        p.maxEdit->setStep(model->getStep());
        p.maxEdit->setLargeStep(model->getLargeStep());

        p.layout = VerticalLayout::create(context);
        p.layout->setMarginRole(SizeRole::MarginSmall);
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.resetButton->setParent(p.layout);
        // A soft range is a starting point the user may adjust, so it is
        // offered here; a hard range is a fixed bound.
        if (model->isRangeSoft())
        {
            auto formLayout = FormLayout::create(context, p.layout);
            formLayout->setSpacingRole(SizeRole::SpacingSmall);
            formLayout->addRow("Minimum:", p.minEdit);
            formLayout->addRow("Maximum:", p.maxEdit);
        }
        setWidget(p.layout);

        std::weak_ptr<IntModel> modelResetWeak(model);
        p.resetButton->setClickedCallback(
            [modelResetWeak]
            {
                if (auto model = modelResetWeak.lock())
                {
                    model->setDefault();
                }
            });

        std::weak_ptr<IntModel> modelWeak(model);
        p.minEdit->setCallback(
            [modelWeak](int value)
            {
                if (auto model = modelWeak.lock())
                {
                    const RangeI& range = model->getRange();
                    model->setRange(RangeI(value, std::max(value, range.max())));
                }
            });
        p.maxEdit->setCallback(
            [modelWeak](int value)
            {
                if (auto model = modelWeak.lock())
                {
                    const RangeI& range = model->getRange();
                    model->setRange(RangeI(std::min(value, range.min()), value));
                }
            });

        p.rangeObserver = Observer<RangeI>::create(
            model->observeRange(),
            [this](const RangeI& value)
            {
                FTK_P();
                p.minEdit->setValue(value.min());
                p.maxEdit->setValue(value.max());
            });
    }

    IntSliderPopup::IntSliderPopup() :
        _p(new Private)
    {}

    IntSliderPopup::~IntSliderPopup()
    {}

    std::shared_ptr<IntSliderPopup> IntSliderPopup::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntSliderPopup>(new IntSliderPopup);
        out->_init(context, model, parent);
        return out;
    }
}
