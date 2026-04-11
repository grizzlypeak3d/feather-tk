// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Observable.h>
#include <ftk/Core/Range.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Integer value model.
    class FTK_API_TYPE IntModel : public std::enable_shared_from_this<IntModel>
    {
        FTK_NON_COPYABLE(IntModel);

    protected:
        IntModel();

    public:
        FTK_API ~IntModel();

        //! Create a new model.
        FTK_API static std::shared_ptr<IntModel> create();

        //! \name Value
        ///@{

        FTK_API int getValue() const;
        FTK_API std::shared_ptr<IObservable<int> > observeValue() const;
        FTK_API void setValue(int);

        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeI& getRange() const;
        FTK_API std::shared_ptr<IObservable<RangeI> > observeRange() const;
        FTK_API void setRange(const RangeI&);

        ///@}

        //! \name Increment
        ///@{

        FTK_API int getStep() const;
        FTK_API void setStep(int);

        FTK_API int getLargeStep() const;
        FTK_API void setLargeStep(int);

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
        FTK_API int getDefault() const;

        //! Observe the default value.
        FTK_API std::shared_ptr<IObservable<int> > observeDefault() const;

        //! Set the default value.
        FTK_API void setDefault(int);

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
