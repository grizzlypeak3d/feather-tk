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
        FTK_API virtual ~IntSlider();

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_API int getValue() const;
        FTK_API void setValue(int);
        FTK_API void setCallback(const std::function<void(int)>&);
        
        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeI& getRange() const;
        FTK_API void setRange(const RangeI&);
        FTK_API void setRange(int, int);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_API int getStep() const;
        FTK_API void setStep(int);

        FTK_API int getLargeStep() const;
        FTK_API void setLargeStep(int);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_API int getDefaultValue() const;
        FTK_API void setDefaultValue(int);
        
        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<IntModel>& getModel() const;

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

        int _posToValue(int) const;
        int _valueToPos(int) const;

        FTK_PRIVATE();
    };
        
    ///@}
}
