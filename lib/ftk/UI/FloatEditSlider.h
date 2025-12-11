// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Floating point value editor and slider.
    class FTK_API_TYPE FloatEditSlider : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatEditSlider();

    public:
        virtual ~FloatEditSlider();

        //! Create a new widget.
        static std::shared_ptr<FloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<FloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the value.
        float getValue() const;

        //! Set the value.
        void setValue(float);

        //! Set the callback.
        void setCallback(const std::function<void(float)>&);

        //! Get the range.
        const RangeF& getRange() const;

        //! Set the range.
        void setRange(const RangeF&);

        //! Set the range.
        void setRange(float, float);

        //! Get the step.
        float getStep() const;

        //! Set the step.
        void setStep(float);

        //! Get the large step.
        float getLargeStep() const;

        //! Set the large step.
        void setLargeStep(float);

        //! Get the default value.
        float getDefaultValue() const;

        //! Set the default value.
        void setDefaultValue(float);

        //! Get the model.
        const std::shared_ptr<FloatModel>& getModel() const;

        //! Get the display precision.
        int getPrecision() const;

        //! Set the display precision.
        void setPrecision(int);

        //! Get the font role.
        FontRole getFontRole() const;

        //! Set the font role.
        void setFontRole(FontRole);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
