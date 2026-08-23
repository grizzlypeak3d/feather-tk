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

        FTK_UI_API int getValue() const;
        FTK_UI_API void setValue(int);
        FTK_UI_API void setCallback(const std::function<void(int)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(int, bool)>&);
        
        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeI& getRange() const;
        FTK_UI_API void setRange(const RangeI&);
        FTK_UI_API void setRange(int, int);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_UI_API int getStep() const;
        FTK_UI_API void setStep(int);

        FTK_UI_API int getLargeStep() const;
        FTK_UI_API void setLargeStep(int);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_UI_API int getDefault() const;
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
