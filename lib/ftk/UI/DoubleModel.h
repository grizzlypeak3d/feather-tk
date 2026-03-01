// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Observable.h>
#include <ftk/Core/Range.h>

namespace ftk
{
    class Context;
    
    //! \name Numeric Widgets
    ///@{
        
    //! Double precision floating point value model.
    class FTK_API_TYPE DoubleModel : public std::enable_shared_from_this<DoubleModel>
    {
        FTK_NON_COPYABLE(DoubleModel);

        void _init(const std::shared_ptr<Context>&);

    protected:
        DoubleModel();

    public:
        FTK_API ~DoubleModel();

        //! Create a new model.
        FTK_API static std::shared_ptr<DoubleModel> create(
            const std::shared_ptr<Context>&);

        //! \name Value
        ///@{

        FTK_API double getValue() const;
        FTK_API std::shared_ptr<IObservable<double> > observeValue() const;
        FTK_API void setValue(double);

        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeD& getRange() const;
        FTK_API std::shared_ptr<IObservable<RangeD> > observeRange() const;
        FTK_API void setRange(const RangeD&);

        ///@}

        //! \name Increment
        ///@{

        //! Get the increment step.
        FTK_API double getStep() const;

        //! Set the increment step.
        FTK_API void setStep(double);

        //! Increment the value by a step.
        FTK_API void step();

        //! Decrement the value by a step.
        FTK_API void stepDec();

        //! Get the increment large step.
        FTK_API double getLargeStep() const;

        //! Set the increment large step.
        FTK_API void setLargeStep(double);

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

        //! Observe the default value.
        FTK_API std::shared_ptr<IObservable<double> > observeDefault() const;

        //! Get the default value.
        FTK_API double getDefault() const;

        //! Set the default value.
        FTK_API void setDefault(double);

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
