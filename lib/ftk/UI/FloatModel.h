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
        ~FloatModel();

        //! Create a new model.
        static std::shared_ptr<FloatModel> create();

        //! \name Value
        ///@{

        float getValue() const;
        std::shared_ptr<IObservable<float> > observeValue() const;
        void setValue(float);

        ///@}

        //! \name Range
        ///@{

        const RangeF& getRange() const;
        std::shared_ptr<IObservable<RangeF> > observeRange() const;
        void setRange(const RangeF&);

        ///@}

        //! \name Increment
        ///@{

        float getStep() const;
        void setStep(float);

        float getLargeStep() const;
        void setLargeStep(float);

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
        float getDefault() const;

        //! Observe the default value.
        std::shared_ptr<IObservable<float> > observeDefault() const;

        //! Set the default value.
        void setDefault(float);

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
