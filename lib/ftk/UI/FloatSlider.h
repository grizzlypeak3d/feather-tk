// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{

    //! \name Numeric Widgets
    ///@{

    //! Base class for floating point sliders.
    class FTK_UI_API_TYPE IFloatSlider : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        IFloatSlider();

    public:
        virtual ~IFloatSlider() = 0;

        //! \name Value
        ///@{

        //! Get the value, which is kept within the range.
        FTK_UI_API float getValue() const;
        //! Set the value. A value outside the range is clamped to it.
        FTK_UI_API void setValue(float);
        //! Set the callback, called with the value whenever it changes,
        //! including on every move while the slider is dragged, from
        //! setValue(), and from other widgets that share the model.
        FTK_UI_API void setCallback(const std::function<void(float)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(float, bool)>&);

        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within.
        FTK_UI_API const RangeF& getRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeF&);
        //! Set the range from its minimum and maximum.
        FTK_UI_API void setRange(float, float);
        
        ///@}

        //! \name Increment
        ///@{

        //! Get the step: how much the arrow keys and the scroll wheel change
        //! the value by.
        FTK_UI_API float getStep() const;
        //! Set the step.
        FTK_UI_API void setStep(float);

        //! Get the large step: how much Page Up and Page Down, and the scroll
        //! wheel with Shift held, change the value by.
        FTK_UI_API float getLargeStep() const;
        //! Set the large step.
        FTK_UI_API void setLargeStep(float);
        
        ///@}

        //! \name Default Value
        ///@{

        //! Get the default value.
        FTK_UI_API float getDefault() const;
        //! Set the default value. A double click on the slider resets the value
        //! to it.
        FTK_UI_API void setDefault(float);
        
        ///@}

        //! Get the model.
        FTK_UI_API const std::shared_ptr<FloatModel>& getModel() const;

        FTK_UI_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_UI_API void mouseLeaveEvent() override;
        FTK_UI_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_UI_API void scrollEvent(ScrollEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    protected:
        FTK_UI_API virtual Box2I _getSliderGeometry() const = 0;

        FTK_UI_API float _posToValue(int) const;
        FTK_UI_API int _valueToPos(float) const;

    private:

        FTK_PRIVATE();
    };

    //! Floating point slider.
    class FTK_UI_API_TYPE FloatSlider : public IFloatSlider
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
        FTK_UI_API static std::shared_ptr<FloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_UI_API Box2I _getSliderGeometry() const override;

    private:
        Box2I _getInsideGeometry() const;
        Box2I _getHandleGeometry() const;

        FTK_PRIVATE();
    };
        
    ///@}
}
