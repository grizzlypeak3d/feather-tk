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
        
    //! Integer value model.
    class FTK_UI_API_TYPE IntModel : public std::enable_shared_from_this<IntModel>
    {
        FTK_NON_COPYABLE(IntModel);

    protected:
        IntModel();

    public:
        FTK_UI_API ~IntModel();

        //! Create a new model.
        FTK_UI_API static std::shared_ptr<IntModel> create();

        //! \name Value
        ///@{

        FTK_UI_API int getValue() const;
        FTK_UI_API std::shared_ptr<IObservable<int> > observeValue() const;
        FTK_UI_API void setValue(int);

        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeI& getRange() const;
        FTK_UI_API std::shared_ptr<IObservable<RangeI> > observeRange() const;
        FTK_UI_API void setRange(const RangeI&);

        //! Get whether the range is soft. With a soft range, setting a
        //! value beyond the range extends the range instead of being
        //! clamped.
        FTK_UI_API bool isRangeSoft() const;

        //! Set whether the range is soft.
        FTK_UI_API void setRangeSoft(bool);

        ///@}

        //! \name Increment
        ///@{

        FTK_UI_API int getStep() const;
        FTK_UI_API void setStep(int);

        FTK_UI_API int getLargeStep() const;
        FTK_UI_API void setLargeStep(int);

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
        FTK_UI_API int getDefault() const;

        //! Observe the default value.
        FTK_UI_API std::shared_ptr<IObservable<int> > observeDefault() const;

        //! Set the default value.
        FTK_UI_API void setDefault(int);

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
