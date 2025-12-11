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
        virtual ~DoubleSlider();

        //! Create a new widget.
        static std::shared_ptr<DoubleSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<DoubleSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the value.
        double getValue() const;

        //! Set the value.
        void setValue(double);

        //! Set the callback.
        void setCallback(const std::function<void(double)>&);

        //! Get the range.
        const RangeD& getRange() const;

        //! Set the range.
        void setRange(const RangeD&);

        //! Set the range.
        void setRange(double, double);

        //! Get the step.
        double getStep() const;

        //! Set the step.
        void setStep(double);

        //! Get the large step.
        double getLargeStep() const;

        //! Set the large step.
        void setLargeStep(double);

        //! Get the default value.
        double getDefaultValue() const;

        //! Set the default value.
        void setDefaultValue(double);

        //! Get the model.
        const std::shared_ptr<DoubleModel>& getModel() const;

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

        double _posToValue(int) const;
        int _valueToPos(double) const;

        FTK_PRIVATE();
    };
        
    ///@}
}
