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

        FTK_UI_API float getValue() const;
        FTK_UI_API void setValue(float);
        FTK_UI_API void setCallback(const std::function<void(float)>&);
        
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

        FTK_UI_API FontType getFont() const;
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
