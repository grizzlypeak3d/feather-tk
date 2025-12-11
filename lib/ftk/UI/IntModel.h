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
        
    //! Integer value model.
    class FTK_API_TYPE IntModel : public std::enable_shared_from_this<IntModel>
    {
        FTK_NON_COPYABLE(IntModel);

        void _init(const std::shared_ptr<Context>&);

    protected:
        IntModel();

    public:
        ~IntModel();

        //! Create a new model.
        static std::shared_ptr<IntModel> create(
            const std::shared_ptr<Context>&);

        //! \name Value
        ///@{

        //! Get the value.
        int getValue() const;

        //! Observe the value.
        std::shared_ptr<IObservable<int> > observeValue() const;

        //! Set the value.
        void setValue(int);

        ///@}

        //! \name Range
        ///@{

        //! Get the range.
        const RangeI& getRange() const;

        //! Observe the range.
        std::shared_ptr<IObservable<RangeI> > observeRange() const;

        //! Set the range.
        void setRange(const RangeI&);

        ///@}

        //! \name Increment
        ///@{

        //! Get the increment step.
        int getStep() const;

        //! Set the increment step.
        void setStep(int);

        //! Increment the value by a step.
        void incrementStep();

        //! Decrement the value by a step.
        void decrementStep();

        //! Get the increment large step.
        int getLargeStep() const;

        //! Set the increment large step.
        void setLargeStep(int);

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
        std::shared_ptr<IObservable<int> > observeDefaultValue() const;

        //! Get the default value.
        int getDefaultValue() const;

        //! Set the default value.
        void setDefaultValue(int);

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
