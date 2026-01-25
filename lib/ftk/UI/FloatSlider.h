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
        FTK_API static std::shared_ptr<FloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<FloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_API float getValue() const;
        FTK_API void setValue(float);
        FTK_API void setCallback(const std::function<void(float)>&);
        
        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeF& getRange() const;
        FTK_API void setRange(const RangeF&);
        FTK_API void setRange(float, float);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_API float getStep() const;
        FTK_API void setStep(float);

        FTK_API float getLargeStep() const;
        FTK_API void setLargeStep(float);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_API float getDefaultValue() const;
        FTK_API void setDefaultValue(float);
        
        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<FloatModel>& getModel() const;

        //! Set the pressed callback.
        FTK_API void setPressedCallback(const std::function<void(bool)>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_API void scrollEvent(ScrollEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        Box2I _getSliderGeometry() const;

        float _posToValue(int) const;
        int _valueToPos(float) const;

        FTK_PRIVATE();
    };
        
    ///@}
}
