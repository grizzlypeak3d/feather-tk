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

        int getValue() const;
        std::shared_ptr<IObservable<int> > observeValue() const;
        void setValue(int);

        ///@}

        //! \name Range
        ///@{

        const RangeI& getRange() const;
        std::shared_ptr<IObservable<RangeI> > observeRange() const;
        void setRange(const RangeI&);

        ///@}

        //! \name Increment
        ///@{

        //! Get the increment step.
        int getStep() const;

        //! Set the increment step.
        void setStep(int);

        //! Increment the value by a step.
        void step();

        //! Decrement the value by a step.
        void stepDec();

        //! Get the increment large step.
        int getLargeStep() const;

        //! Set the increment large step.
        void setLargeStep(int);

        //! Increment the value by a large step.
        void largeStep();

        //! Decrement the value by a large step.
        void largeStepDec();

        ///@}

        //! \name Default Value
        ///@{

        //! Get whether there is a default value.
        bool hasDefault() const;

        //! Observe whether there is a default value.
        std::shared_ptr<IObservable<bool> > observeHasDefault() const;

        //! Observe the default value.
        std::shared_ptr<IObservable<int> > observeDefault() const;

        //! Get the default value.
        int getDefault() const;

        //! Set the default value.
        void setDefault(int);

        //! Set the value to the default value.
        void setDefault();

        //! Clear the default value.
        void clearDefault();

        ///@}

    private:
        FTK_PRIVATE();
    };

    ///@}
}
