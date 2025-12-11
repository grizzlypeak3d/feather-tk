// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{

    //! Floating point value slider.
    class FTK_API_TYPE FloatSlider : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatSlider();

    public:
        virtual ~FloatSlider();

        //! Create a new widget.
        static std::shared_ptr<FloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<FloatSlider> create(
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

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mouseEnterEvent(MouseEnterEvent&) override;
        void mouseLeaveEvent() override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;
        void scrollEvent(ScrollEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        Box2I _getSliderGeometry() const;

        float _posToValue(int) const;
        int _valueToPos(float) const;

        FTK_PRIVATE();
    };
        
    ///@}
}
