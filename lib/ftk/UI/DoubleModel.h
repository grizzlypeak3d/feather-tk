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
        ~DoubleModel();

        //! Create a new model.
        static std::shared_ptr<DoubleModel> create(
            const std::shared_ptr<Context>&);

        //! \name Value
        ///@{

        //! Get the value.
        double getValue() const;

        //! Observe the value.
        std::shared_ptr<IObservable<double> > observeValue() const;

        //! Set the value.
        void setValue(double);

        ///@}

        //! \name Range
        ///@{

        //! Get the range.
        const RangeD& getRange() const;

        //! Observe the range.
        std::shared_ptr<IObservable<RangeD> > observeRange() const;

        //! Set the range.
        void setRange(const RangeD&);

        ///@}

        //! \name Increment
        ///@{

        //! Get the increment step.
        double getStep() const;

        //! Set the increment step.
        void setStep(double);

        //! Increment the value by a step.
        void incrementStep();

        //! Decrement the value by a step.
        void decrementStep();

        //! Get the increment large step.
        double getLargeStep() const;

        //! Set the increment large step.
        void setLargeStep(double);

        //! Increment the value by a large step.
        void incrementLargeStep();

        //! Decrement the value by a large step.
        void decrementLargeStep();

        ///@}

        //! \name Default Value
        ///@{

        //! Get whether there is a default value.
        bool hasDefaultValue() const;

        //! Observe whether there is a default value.
        std::shared_ptr<IObservable<bool> > observeHasDefaultValue() const;

        //! Observe the default value.
        std::shared_ptr<IObservable<double> > observeDefaultValue() const;

        //! Get the default value.
        double getDefaultValue() const;

        //! Set the default value.
        void setDefaultValue(double);

        //! Set the value to the default value.
        void setDefaultValue();

        //! Clear the default value.
        void clearDefaultValue();

        ///@}

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
