// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/Core/Observable.h>
#include <ftk/Core/Range.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{

    //! Floating point value model.
    class FTK_UI_API_TYPE FloatModel : public std::enable_shared_from_this<FloatModel>
    {
        FTK_NON_COPYABLE(FloatModel);

    protected:
        FloatModel();

    public:
        FTK_UI_API ~FloatModel();

        //! Create a new model.
        FTK_UI_API static std::shared_ptr<FloatModel> create();

        //! \name Value
        ///@{

        FTK_UI_API float getValue() const;
        FTK_UI_API std::shared_ptr<IObservable<float> > observeValue() const;
        FTK_UI_API void setValue(float);

        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeF& getRange() const;
        FTK_UI_API std::shared_ptr<IObservable<RangeF> > observeRange() const;
        FTK_UI_API void setRange(const RangeF&);

        ///@}

        //! \name Increment
        ///@{

        FTK_UI_API float getStep() const;
        FTK_UI_API void setStep(float);

        FTK_UI_API float getLargeStep() const;
        FTK_UI_API void setLargeStep(float);

        //! Increment the value by a step.
        FTK_UI_API void step();

        //! Decrement the value by a step.
        FTK_UI_API void stepDec();

        //! Increment the value by a large step.
        FTK_UI_API void largeStep();

        //! Decrement the value by a large step.
        FTK_UI_API void largeStepDec();

        ///@}

        //! \name Default Value
        ///@{

        //! Get whether there is a default value.
        FTK_UI_API bool hasDefault() const;

        //! Observe whether there is a default value.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeHasDefault() const;

        //! Get the default value.
        FTK_UI_API float getDefault() const;

        //! Observe the default value.
        FTK_UI_API std::shared_ptr<IObservable<float> > observeDefault() const;

        //! Set the default value.
        FTK_UI_API void setDefault(float);

        //! Set the value to the default value.
        FTK_UI_API void setDefault();

        //! Clear the default value.
        FTK_UI_API void clearDefault();

        ///@}

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
