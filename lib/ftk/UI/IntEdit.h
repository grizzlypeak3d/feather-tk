// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>
#include <ftk/UI/IntModel.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{

    //! Integer value editor.
    class FTK_API_TYPE IntEdit : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntEdit();

    public:
        FTK_API virtual ~IntEdit();

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntEdit> create(
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

        //! Get the font role.
        FTK_API FontRole getFontRole() const;

        //! Set the font role.
        FTK_API void setFontRole(FontRole);

        FTK_API void takeKeyFocus() override;
        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void scrollEvent(ScrollEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _textUpdate();

        FTK_PRIVATE();
    };

    //! Integer reset button.
    class FTK_API_TYPE IntResetButton : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntResetButton();

    public:
        FTK_API virtual ~IntResetButton();

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntResetButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
