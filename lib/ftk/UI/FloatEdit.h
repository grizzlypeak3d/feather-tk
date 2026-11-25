// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Floating point value editor.
    class FTK_UI_API_TYPE FloatEdit : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatEdit();

    public:
        FTK_UI_API virtual ~FloatEdit();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FloatEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FloatEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_UI_API const std::shared_ptr<FloatModel>& getModel() const;

        //! \name Value
        ///@{

        //! Get the value, which is kept within the range.
        FTK_UI_API float getValue() const;
        //! Set the value. A value outside the range is clamped to it.
        FTK_UI_API void setValue(float);
        //! Set the callback, called with the value whenever it changes,
        //! including from setValue() and from other widgets that share the
        //! model. Typed text is applied when Enter is pressed or the key focus
        //! is lost.
        FTK_UI_API void setCallback(const std::function<void(float)>&);
        
        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within.
        FTK_UI_API const RangeF& getRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeF&);
        //! Set the range from its minimum and maximum.
        FTK_UI_API void setRange(float, float);
        
        ///@}

        //! \name Increment
        ///@{

        //! Get the step: how much the up and down arrow keys, the increment
        //! buttons, and the scroll wheel change the value by.
        FTK_UI_API float getStep() const;
        //! Set the step.
        FTK_UI_API void setStep(float);

        //! Get the large step: how much Page Up and Page Down, and the scroll
        //! wheel with Shift held, change the value by.
        FTK_UI_API float getLargeStep() const;
        //! Set the large step.
        FTK_UI_API void setLargeStep(float);
        
        ///@}

        //! \name Default Value
        ///@{

        //! Get the default value.
        FTK_UI_API float getDefault() const;
        //! Set the default value.
        FTK_UI_API void setDefault(float);
        
        ///@}

        //! \name Text
        ///@{

        //! Get the display precision.
        FTK_UI_API int getPrecision() const;

        //! Set the display precision.
        FTK_UI_API void setPrecision(int);

        //! Get the formatting text.
        FTK_UI_API const std::string& getFormat() const;

        //! Set the formatting text.
        FTK_UI_API void setFormat(const std::string&);

        //! Get the font, the monospace font by default.
        FTK_UI_API FontType getFont() const;
        //! Set the font.
        FTK_UI_API void setFont(FontType);

        ///@}

        FTK_UI_API void takeKeyFocus() override;
        void scrollEvent(ScrollEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        void _textUpdate();

        FTK_PRIVATE();
    };

    //! Floating point reset button.
    class FTK_UI_API_TYPE FloatResetButton : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatResetButton();

    public:
        FTK_UI_API virtual ~FloatResetButton();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FloatResetButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
