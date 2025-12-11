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
        virtual ~IntEdit();

        //! Create a new widget.
        static std::shared_ptr<IntEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<IntEdit> create(
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

        //! Get the font role.
        FontRole getFontRole() const;

        //! Set the font role.
        void setFontRole(FontRole);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void scrollEvent(ScrollEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

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
        virtual ~IntResetButton();

        //! Create a new widget.
        static std::shared_ptr<IntResetButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
