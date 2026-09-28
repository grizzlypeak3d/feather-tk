// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/DoubleModel.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Double precision floating point value editor.
    class FTK_UI_API_TYPE DoubleEdit : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleEdit();

    public:
        FTK_UI_API virtual ~DoubleEdit();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<DoubleEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<DoubleEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_UI_API const std::shared_ptr<DoubleModel>& getModel() const;

        //! \name Value
        ///@{

        FTK_UI_API double getValue() const;
        FTK_UI_API void setValue(double);
        FTK_UI_API void setCallback(const std::function<void(double)>&);
        
        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeD& getRange() const;
        FTK_UI_API void setRange(const RangeD&);
        FTK_UI_API void setRange(double, double);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_UI_API double getStep() const;
        FTK_UI_API void setStep(double);

        FTK_UI_API double getLargeStep() const;
        FTK_UI_API void setLargeStep(double);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_UI_API double getDefault() const;
        FTK_UI_API void setDefault(double);
        
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

        //! Get the border role.
        FTK_UI_API ColorRole getBorderRole() const;

        //! Set the border role.
        FTK_UI_API void setBorderRole(ColorRole);

        FTK_UI_API void takeKeyFocus() override;
        FTK_UI_API void scrollEvent(ScrollEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _textUpdate();

        FTK_PRIVATE();
    };

    //! Double precision floating point reset button.
    class FTK_UI_API_TYPE DoubleResetButton : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleResetButton();

    public:
        FTK_UI_API virtual ~DoubleResetButton();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<DoubleResetButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
