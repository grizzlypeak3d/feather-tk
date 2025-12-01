// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntModel.h>

#include <ftk/Core/Math.h>

namespace ftk
{
    struct IntModel::Private
    {
        std::shared_ptr<Observable<int> > value;
        std::shared_ptr<Observable<RangeI> > range;
        int step = 1;
        int largeStep = 10;
        std::shared_ptr<Observable<bool> > hasDefaultValue;
        std::shared_ptr<Observable<int> > defaultValue;
    };

    void IntModel::_init(const std::shared_ptr<Context>&)
    {
        FTK_P();
        p.value = Observable<int>::create(0);
        p.range = Observable<RangeI>::create(RangeI(0, 100));
        p.hasDefaultValue = Observable<bool>::create(false);
        p.defaultValue = Observable<int>::create(0);
    }

    IntModel::IntModel() :
        _p(new Private)
    {}

    IntModel::~IntModel()
    {}

    std::shared_ptr<IntModel> IntModel::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<IntModel>(new IntModel);
        out->_init(context);
        return out;
    }

    int IntModel::getValue() const
    {
        return _p->value->get();
    }

    std::shared_ptr<IObservable<int> > IntModel::observeValue() const
    {
        return _p->value;
    }

    void IntModel::setValue(int value)
    {
        FTK_P();
        const RangeI& range = p.range->get();
        const int tmp = clamp(value, range.min(), range.max());
        _p->value->setIfChanged(tmp);
    }

    const RangeI& IntModel::getRange() const
    {
        return _p->range->get();
    }

    std::shared_ptr<IObservable<RangeI> > IntModel::observeRange() const
    {
        return _p->range;
    }

    void IntModel::setRange(const RangeI& range)
    {
        FTK_P();
        if (p.range->setIfChanged(range))
        {
            setValue(p.value->get());
        }
    }

    int IntModel::getStep() const
    {
        return _p->step;
    }

    void IntModel::setStep(int value)
    {
        _p->step = value;
    }

    void IntModel::incrementStep()
    {
        FTK_P();
        setValue(p.value->get() + p.step);
    }

    void IntModel::decrementStep()
    {
        FTK_P();
        setValue(p.value->get() - p.step);
    }

    int IntModel::getLargeStep() const
    {
        return _p->largeStep;
    }

    void IntModel::setLargeStep(int value)
    {
        _p->largeStep = value;
    }

    void IntModel::incrementLargeStep()
    {
        FTK_P();
        setValue(p.value->get() + p.largeStep);
    }

    void IntModel::decrementLargeStep()
    {
        FTK_P();
        setValue(p.value->get() - p.largeStep);
    }

    bool IntModel::hasDefaultValue() const
    {
        return _p->hasDefaultValue->get();
    }

    std::shared_ptr<IObservable<bool> > IntModel::observeHasDefaultValue() const
    {
        return _p->hasDefaultValue;
    }

    int IntModel::getDefaultValue() const
    {
        return _p->defaultValue->get();
    }

    std::shared_ptr<IObservable<int> > IntModel::observeDefaultValue() const
    {
        return _p->defaultValue;
    }

    void IntModel::setDefaultValue(int value)
    {
        _p->defaultValue->setIfChanged(value);
        _p->hasDefaultValue->setIfChanged(true);
    }

    void IntModel::setDefaultValue()
    {
        setValue(_p->defaultValue->get());
    }

    void IntModel::clearDefaultValue()
    {
        _p->hasDefaultValue->setIfChanged(false);
    }
}