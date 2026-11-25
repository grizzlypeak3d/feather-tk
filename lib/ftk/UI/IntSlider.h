// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/IntModel.h>

namespace ftk
{

    //! \name Numeric Widgets
    ///@{
        
    //! Base class for integer sliders.
    class FTK_UI_API_TYPE IIntSlider : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IIntSlider();

    public:
        FTK_UI_API virtual ~IIntSlider() = 0;

        //! \name Value
        ///@{

        //! Get the value, which is kept within the range.
        FTK_UI_API int getValue() const;
        //! Set the value. A value outside the range is clamped to it.
        FTK_UI_API void setValue(int);
        //! Set the callback, called with the value whenever it changes,
        //! including on every move while the slider is dragged, from
        //! setValue(), and from other widgets that share the model.
        FTK_UI_API void setCallback(const std::function<void(int)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(int, bool)>&);
        
        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within.
        FTK_UI_API const RangeI& getRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeI&);
        //! Set the range from its minimum and maximum.
        FTK_UI_API void setRange(int, int);
        
        ///@}

        //! \name Increment
        ///@{

        //! Get the step: how much the arrow keys and the scroll wheel change
        //! the value by.
        FTK_UI_API int getStep() const;
        //! Set the step.
        FTK_UI_API void setStep(int);

        //! Get the large step: how much Page Up and Page Down, and the scroll
        //! wheel with Shift held, change the value by.
        FTK_UI_API int getLargeStep() const;
        //! Set the large step.
        FTK_UI_API void setLargeStep(int);
        
        ///@}

        //! \name Default Value
        ///@{

        //! Get the default value.
        FTK_UI_API int getDefault() const;
        //! Set the default value. A double click on the slider resets the value
        //! to it.
        FTK_UI_API void setDefault(int);
        
        ///@}

        //! Get the model.
        FTK_UI_API const std::shared_ptr<IntModel>& getModel() const;

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

        FTK_UI_API int _posToValue(int) const;
        FTK_UI_API int _valueToPos(int) const;

    private:

        FTK_PRIVATE();
    };

    //! Integer slider.
    class FTK_UI_API_TYPE IntSlider : public IIntSlider
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntSlider();

    public:
        FTK_UI_API virtual ~IntSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_UI_API virtual Box2I _getSliderGeometry() const override;

    private:
        Box2I _getInsideGeometry() const;
        Box2I _getHandleGeometry() const;

        FTK_PRIVATE();
    };
        
    ///@}
}
