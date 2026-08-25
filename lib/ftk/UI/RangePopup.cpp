// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/RangePopup.h>

#include <ftk/UI/DoubleEdit.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/IntEdit.h>

#include <limits>

namespace ftk
{
    struct FloatRangePopup::Private
    {
        std::shared_ptr<FloatEdit> minEdit;
        std::shared_ptr<FloatEdit> maxEdit;
        std::shared_ptr<FormLayout> layout;
        std::shared_ptr<Observer<RangeF> > rangeObserver;
    };

    void FloatRangePopup::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidgetPopup::_init(context, "ftk::FloatRangePopup", parent);
        FTK_P();

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

        p.layout = FormLayout::create(context);
        p.layout->setMarginRole(SizeRole::MarginSmall);
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.layout->addRow("Minimum:", p.minEdit);
        p.layout->addRow("Maximum:", p.maxEdit);
        setWidget(p.layout);

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

    FloatRangePopup::FloatRangePopup() :
        _p(new Private)
    {}

    FloatRangePopup::~FloatRangePopup()
    {}

    std::shared_ptr<FloatRangePopup> FloatRangePopup::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatRangePopup>(new FloatRangePopup);
        out->_init(context, model, parent);
        return out;
    }

    struct DoubleRangePopup::Private
    {
        std::shared_ptr<DoubleEdit> minEdit;
        std::shared_ptr<DoubleEdit> maxEdit;
        std::shared_ptr<FormLayout> layout;
        std::shared_ptr<Observer<RangeD> > rangeObserver;
    };

    void DoubleRangePopup::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidgetPopup::_init(context, "ftk::DoubleRangePopup", parent);
        FTK_P();

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

        p.layout = FormLayout::create(context);
        p.layout->setMarginRole(SizeRole::MarginSmall);
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.layout->addRow("Minimum:", p.minEdit);
        p.layout->addRow("Maximum:", p.maxEdit);
        setWidget(p.layout);

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

    DoubleRangePopup::DoubleRangePopup() :
        _p(new Private)
    {}

    DoubleRangePopup::~DoubleRangePopup()
    {}

    std::shared_ptr<DoubleRangePopup> DoubleRangePopup::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleRangePopup>(new DoubleRangePopup);
        out->_init(context, model, parent);
        return out;
    }

    struct IntRangePopup::Private
    {
        std::shared_ptr<IntEdit> minEdit;
        std::shared_ptr<IntEdit> maxEdit;
        std::shared_ptr<FormLayout> layout;
        std::shared_ptr<Observer<RangeI> > rangeObserver;
    };

    void IntRangePopup::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidgetPopup::_init(context, "ftk::IntRangePopup", parent);
        FTK_P();

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

        p.layout = FormLayout::create(context);
        p.layout->setMarginRole(SizeRole::MarginSmall);
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.layout->addRow("Minimum:", p.minEdit);
        p.layout->addRow("Maximum:", p.maxEdit);
        setWidget(p.layout);

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

    IntRangePopup::IntRangePopup() :
        _p(new Private)
    {}

    IntRangePopup::~IntRangePopup()
    {}

    std::shared_ptr<IntRangePopup> IntRangePopup::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntRangePopup>(new IntRangePopup);
        out->_init(context, model, parent);
        return out;
    }
}
