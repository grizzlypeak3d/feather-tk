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
        ~IntModel();

        //! Create a new model.
        static std::shared_ptr<IntModel> create();

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

        int getStep() const;
        void setStep(int);

        int getLargeStep() const;
        void setLargeStep(int);

        //! Increment the value by a step.
        void step();

        //! Decrement the value by a step.
        void stepDec();

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

        //! Get the default value.
        int getDefault() const;

        //! Observe the default value.
        std::shared_ptr<IObservable<int> > observeDefault() const;

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
