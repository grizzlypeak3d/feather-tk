// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DoubleModel.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Double precision floating point value slider.
    class FTK_API_TYPE DoubleSlider : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleSlider();

    public:
        FTK_API virtual ~DoubleSlider();

        //! Create a new widget.
        FTK_API static std::shared_ptr<DoubleSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<DoubleSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_API double getValue() const;
        FTK_API void setValue(double);
        FTK_API void setCallback(const std::function<void(double)>&);
        
        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeD& getRange() const;
        FTK_API void setRange(const RangeD&);
        FTK_API void setRange(double, double);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_API double getStep() const;
        FTK_API void setStep(double);

        FTK_API double getLargeStep() const;
        FTK_API void setLargeStep(double);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_API double getDefaultValue() const;
        FTK_API void setDefaultValue(double);
        
        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<DoubleModel>& getModel() const;

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

        double _posToValue(int) const;
        int _valueToPos(double) const;

        FTK_PRIVATE();
    };
        
    ///@}
}
