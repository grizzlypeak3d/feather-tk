// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Observable.h>
#include <ftk/Core/Range.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{

    //! Floating point value model.
    class FTK_API_TYPE FloatModel : public std::enable_shared_from_this<FloatModel>
    {
        FTK_NON_COPYABLE(FloatModel);

    protected:
        FloatModel();

    public:
        FTK_API ~FloatModel();

        //! Create a new model.
        FTK_API static std::shared_ptr<FloatModel> create();

        //! \name Value
        ///@{

        FTK_API float getValue() const;
        FTK_API std::shared_ptr<IObservable<float> > observeValue() const;
        FTK_API void setValue(float);

        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeF& getRange() const;
        FTK_API std::shared_ptr<IObservable<RangeF> > observeRange() const;
        FTK_API void setRange(const RangeF&);

        ///@}

        //! \name Increment
        ///@{

        FTK_API float getStep() const;
        FTK_API void setStep(float);

        FTK_API float getLargeStep() const;
        FTK_API void setLargeStep(float);

        //! Increment the value by a step.
        FTK_API void step();

        //! Decrement the value by a step.
        FTK_API void stepDec();

        //! Increment the value by a large step.
        FTK_API void largeStep();

        //! Decrement the value by a large step.
        FTK_API void largeStepDec();

        ///@}

        //! \name Default Value
        ///@{

        //! Get whether there is a default value.
        FTK_API bool hasDefault() const;

        //! Observe whether there is a default value.
        FTK_API std::shared_ptr<IObservable<bool> > observeHasDefault() const;

        //! Get the default value.
        FTK_API float getDefault() const;

        //! Observe the default value.
        FTK_API std::shared_ptr<IObservable<float> > observeDefault() const;

        //! Set the default value.
        FTK_API void setDefault(float);

        //! Set the value to the default value.
        FTK_API void setDefault();

        //! Clear the default value.
        FTK_API void clearDefault();

        ///@}

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
