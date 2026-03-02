// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DoubleModel.h>

#include <ftk/Core/Math.h>

namespace ftk
{
    struct DoubleModel::Private
    {
        std::shared_ptr<Observable<double> > value;
        std::shared_ptr<Observable<RangeD> > range;
        double step = 0.1;
        double largeStep = 1.0;
        std::shared_ptr<Observable<bool> > hasDefault;
        std::shared_ptr<Observable<double> > defaultValue;
    };

    DoubleModel::DoubleModel() :
        _p(new Private)
    {
        FTK_P();
        p.value = Observable<double>::create(0.0);
        p.range = Observable<RangeD>::create(RangeD(0.0, 1.0));
        p.hasDefault = Observable<bool>::create(false);
        p.defaultValue = Observable<double>::create(0.0);
    }

    DoubleModel::~DoubleModel()
    {}

    std::shared_ptr<DoubleModel> DoubleModel::create()
    {
        return std::shared_ptr<DoubleModel>(new DoubleModel);
    }

    double DoubleModel::getValue() const
    {
        return _p->value->get();
    }

    std::shared_ptr<IObservable<double> > DoubleModel::observeValue() const
    {
        return _p->value;
    }

    void DoubleModel::setValue(double value)
    {
        FTK_P();
        const RangeD& range = p.range->get();
        const double tmp = clamp(value, range.min(), range.max());
        _p->value->setIfChanged(tmp);
    }

    const RangeD& DoubleModel::getRange() const
    {
        return _p->range->get();
    }

    std::shared_ptr<IObservable<RangeD> > DoubleModel::observeRange() const
    {
        return _p->range;
    }

    void DoubleModel::setRange(const RangeD& range)
    {
        FTK_P();
        if (p.range->setIfChanged(range))
        {
            setValue(p.value->get());
        }
    }

    double DoubleModel::getStep() const
    {
        return _p->step;
    }

    void DoubleModel::setStep(double value)
    {
        _p->step = value;
    }

    double DoubleModel::getLargeStep() const
    {
        return _p->largeStep;
    }

    void DoubleModel::setLargeStep(double value)
    {
        _p->largeStep = value;
    }

    void DoubleModel::step()
    {
        FTK_P();
        setValue(p.value->get() + p.step);
    }

    void DoubleModel::stepDec()
    {
        FTK_P();
        setValue(p.value->get() - p.step);
    }

    void DoubleModel::largeStep()
    {
        FTK_P();
        setValue(p.value->get() + p.largeStep);
    }

    void DoubleModel::largeStepDec()
    {
        FTK_P();
        setValue(p.value->get() - p.largeStep);
    }

    bool DoubleModel::hasDefault() const
    {
        return _p->hasDefault->get();
    }

    std::shared_ptr<IObservable<bool> > DoubleModel::observeHasDefault() const
    {
        return _p->hasDefault;
    }

    double DoubleModel::getDefault() const
    {
        return _p->defaultValue->get();
    }

    std::shared_ptr<IObservable<double> > DoubleModel::observeDefault() const
    {
        return _p->defaultValue;
    }

    void DoubleModel::setDefault(double value)
    {
        _p->defaultValue->setIfChanged(value);
        _p->hasDefault->setIfChanged(true);
    }

    void DoubleModel::setDefault()
    {
        setValue(_p->defaultValue->get());
    }

    void DoubleModel::clearDefault()
    {
        _p->hasDefault->setIfChanged(false);
    }
}