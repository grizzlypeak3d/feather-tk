// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/IWidget.h>
#include <ftk/UI/IntModel.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{

    //! Integer value editor.
    class FTK_UI_API_TYPE IntEdit : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntEdit();

    public:
        FTK_UI_API virtual ~IntEdit();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_UI_API const std::shared_ptr<IntModel>& getModel() const;

        //! \name Value
        ///@{

        FTK_UI_API int getValue() const;
        FTK_UI_API void setValue(int);
        FTK_UI_API void setCallback(const std::function<void(int)>&);
        
        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeI& getRange() const;
        FTK_UI_API void setRange(const RangeI&);
        FTK_UI_API void setRange(int, int);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_UI_API int getStep() const;
        FTK_UI_API void setStep(int);

        FTK_UI_API int getLargeStep() const;
        FTK_UI_API void setLargeStep(int);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_UI_API int getDefault() const;
        FTK_UI_API void setDefault(int);
        
        ///@}

        //! \name Text
        ///@{

        //! Get the formatting text.
        FTK_UI_API const std::string& getFormat() const;

        //! Set the formatting text.
        FTK_UI_API void setFormat(const std::string&);

        FTK_UI_API FontType getFont() const;
        FTK_UI_API void setFont(FontType);

        ///@}

        FTK_UI_API void takeKeyFocus() override;
        FTK_UI_API void scrollEvent(ScrollEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _textUpdate();

        FTK_PRIVATE();
    };

    //! Integer reset button.
    class FTK_UI_API_TYPE IntResetButton : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntResetButton();

    public:
        FTK_UI_API virtual ~IntResetButton();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntResetButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
