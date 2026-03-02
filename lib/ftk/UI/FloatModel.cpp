// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FloatModel.h>

#include <ftk/Core/Math.h>

namespace ftk
{
    struct FloatModel::Private
    {
        std::shared_ptr<Observable<float> > value;
        std::shared_ptr<Observable<RangeF> > range;
        float step = .1F;
        float largeStep = 1.F;
        std::shared_ptr<Observable<bool> > hasDefault;
        std::shared_ptr<Observable<float> > defaultValue;
    };

    FloatModel::FloatModel() :
        _p(new Private)
    {
        FTK_P();
        p.value = Observable<float>::create(0.F);
        p.range = Observable<RangeF>::create(RangeF(0.F, 1.F));
        p.hasDefault = Observable<bool>::create(false);
        p.defaultValue = Observable<float>::create(0.F);
    }

    FloatModel::~FloatModel()
    {}

    std::shared_ptr<FloatModel> FloatModel::create()
    {
        return std::shared_ptr<FloatModel>(new FloatModel);
    }

    float FloatModel::getValue() const
    {
        return _p->value->get();
    }

    std::shared_ptr<IObservable<float> > FloatModel::observeValue() const
    {
        return _p->value;
    }

    void FloatModel::setValue(float value)
    {
        FTK_P();
        const RangeF& range = p.range->get();
        const float tmp = clamp(value, range.min(), range.max());
        _p->value->setIfChanged(tmp);
    }

    const RangeF& FloatModel::getRange() const
    {
        return _p->range->get();
    }

    std::shared_ptr<IObservable<RangeF> > FloatModel::observeRange() const
    {
        return _p->range;
    }

    void FloatModel::setRange(const RangeF& range)
    {
        FTK_P();
        if (p.range->setIfChanged(range))
        {
            setValue(p.value->get());
        }
    }

    float FloatModel::getStep() const
    {
        return _p->step;
    }

    void FloatModel::setStep(float value)
    {
        _p->step = value;
    }

    float FloatModel::getLargeStep() const
    {
        return _p->largeStep;
    }

    void FloatModel::setLargeStep(float value)
    {
        _p->largeStep = value;
    }

    void FloatModel::step()
    {
        FTK_P();
        setValue(p.value->get() + p.step);
    }

    void FloatModel::stepDec()
    {
        FTK_P();
        setValue(p.value->get() - p.step);
    }

    void FloatModel::largeStep()
    {
        FTK_P();
        setValue(p.value->get() + p.largeStep);
    }

    void FloatModel::largeStepDec()
    {
        FTK_P();
        setValue(p.value->get() - p.largeStep);
    }

    bool FloatModel::hasDefault() const
    {
        return _p->hasDefault->get();
    }

    std::shared_ptr<IObservable<bool> > FloatModel::observeHasDefault() const
    {
        return _p->hasDefault;
    }

    float FloatModel::getDefault() const
    {
        return _p->defaultValue->get();
    }

    std::shared_ptr<IObservable<float> > FloatModel::observeDefault() const
    {
        return _p->defaultValue;
    }

    void FloatModel::setDefault(float value)
    {
        _p->defaultValue->setIfChanged(value);
        _p->hasDefault->setIfChanged(true);
    }

    void FloatModel::setDefault()
    {
        setValue(_p->defaultValue->get());
    }

    void FloatModel::clearDefault()
    {
        _p->hasDefault->setIfChanged(false);
    }
}