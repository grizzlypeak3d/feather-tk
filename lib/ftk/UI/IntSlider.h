// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/IntModel.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Integer value slider.
    class FTK_API_TYPE IntSlider : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntSlider();

    public:
        virtual ~IntSlider();

        //! Create a new widget.
        static std::shared_ptr<IntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<IntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the value.
        int getValue() const;

        //! Set the value.
        void setValue(int);

        //! Set the callback.
        void setCallback(const std::function<void(int)>&);

        //! Get the range.
        const RangeI& getRange() const;

        //! Set the range.
        void setRange(const RangeI&);

        //! Set the range.
        void setRange(int, int);

        //! Get the step.
        int getStep() const;

        //! Set the step.
        void setStep(int);

        //! Get the large step.
        int getLargeStep() const;

        //! Set the large step.
        void setLargeStep(int);

        //! Get the default value.
        int getDefaultValue() const;

        //! Set the default value.
        void setDefaultValue(int);

        //! Get the model.
        const std::shared_ptr<IntModel>& getModel() const;

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

        int _posToValue(int) const;
        int _valueToPos(int) const;

        FTK_PRIVATE();
    };
        
    ///@}
}
