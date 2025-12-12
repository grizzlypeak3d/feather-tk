// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DoubleModel.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Double precision floating point value editor.
    class FTK_API_TYPE DoubleEdit : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleEdit();

    public:
        FTK_API virtual ~DoubleEdit();

        //! Create a new widget.
        FTK_API static std::shared_ptr<DoubleEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<DoubleEdit> create(
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

        //! Get the display precision.
        FTK_API int getPrecision() const;

        //! Set the display precision.
        FTK_API void setPrecision(int);

        //! Get the font role.
        FTK_API FontRole getFontRole() const;

        //! Set the font role.
        FTK_API void setFontRole(FontRole);

        //! Get the border role.
        FTK_API ColorRole getBorderRole() const;

        //! Set the border role.
        FTK_API void setBorderRole(ColorRole);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void scrollEvent(ScrollEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _textUpdate();

        FTK_PRIVATE();
    };

    //! Double precision floating point reset button.
    class FTK_API_TYPE DoubleResetButton : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleResetButton();

    public:
        FTK_API virtual ~DoubleResetButton();

        //! Create a new widget.
        FTK_API static std::shared_ptr<DoubleResetButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
