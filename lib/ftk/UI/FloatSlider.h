// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{

    //! Base class for floating point sliders.
    class FTK_API_TYPE IFloatSlider : public IMouseWidget
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

        FTK_API float getValue() const;
        FTK_API void setValue(float);
        FTK_API void setCallback(const std::function<void(float)>&);

        //! Set the value callback with a flag for whether the slider is presssed.
        FTK_API void setPressedCallback(const std::function<void(float, bool)>&);

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

        FTK_API float getDefault() const;
        FTK_API void setDefault(float);
        
        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<FloatModel>& getModel() const;

        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_API void scrollEvent(ScrollEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    protected:
        FTK_API virtual Box2I _getSliderGeometry() const = 0;

        FTK_API float _posToValue(int) const;
        FTK_API int _valueToPos(float) const;

    private:
        FTK_PRIVATE();
    };

    //! Floating point slider.
    class FTK_API_TYPE FloatSlider : public IFloatSlider
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

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_API Box2I _getSliderGeometry() const override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
