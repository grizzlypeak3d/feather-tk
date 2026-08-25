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
        
    //! Double precision floating point value model.
    class FTK_UI_API_TYPE DoubleModel : public std::enable_shared_from_this<DoubleModel>
    {
        FTK_NON_COPYABLE(DoubleModel);

    protected:
        DoubleModel();

    public:
        FTK_UI_API ~DoubleModel();

        //! Create a new model.
        FTK_UI_API static std::shared_ptr<DoubleModel> create();

        //! \name Value
        ///@{

        FTK_UI_API double getValue() const;
        FTK_UI_API std::shared_ptr<IObservable<double> > observeValue() const;
        FTK_UI_API void setValue(double);

        //! Set the value, extending a soft range to admit it. This is
        //! for values the user typed; interactive changes such as
        //! dragging use setValue(), which always clamps, so they cannot
        //! run the range away.
        FTK_UI_API void setValueSoft(double);

        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeD& getRange() const;
        FTK_UI_API std::shared_ptr<IObservable<RangeD> > observeRange() const;
        FTK_UI_API void setRange(const RangeD&);

        //! Get whether the range is soft. A value given to
        //! setValueSoft() beyond a soft range extends the range instead
        //! of being clamped.
        FTK_UI_API bool isRangeSoft() const;

        //! Set whether the range is soft.
        FTK_UI_API void setRangeSoft(bool);

        ///@}

        //! \name Increment
        ///@{

        FTK_UI_API double getStep() const;
        FTK_UI_API void setStep(double);

        FTK_UI_API double getLargeStep() const;
        FTK_UI_API void setLargeStep(double);

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
        FTK_UI_API double getDefault() const;

        //! Observe the default value.
        FTK_UI_API std::shared_ptr<IObservable<double> > observeDefault() const;

        //! Set the default value.
        FTK_UI_API void setDefault(double);

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
