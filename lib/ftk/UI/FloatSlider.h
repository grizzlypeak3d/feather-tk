// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    class Menu;

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

        FTK_UI_API float getValue() const;
        FTK_UI_API void setValue(float);
        FTK_UI_API void setCallback(const std::function<void(float)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(float, bool)>&);

        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeF& getRange() const;
        FTK_UI_API void setRange(const RangeF&);
        FTK_UI_API void setRange(float, float);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_UI_API float getStep() const;
        FTK_UI_API void setStep(float);

        FTK_UI_API float getLargeStep() const;
        FTK_UI_API void setLargeStep(float);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_UI_API float getDefault() const;
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

        FTK_UI_API virtual std::shared_ptr<Menu> _createContextMenu();

    private:
        void _showRangePopup();

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
