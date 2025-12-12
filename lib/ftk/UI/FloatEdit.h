// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Floating point value editor.
    class FTK_API_TYPE FloatEdit : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatEdit();

    public:
        FTK_API virtual ~FloatEdit();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FloatEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<FloatEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_API float getValue() const;
        FTK_API void setValue(float);
        FTK_API void setCallback(const std::function<void(float)>&);
        
        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeF& getRange() const;
        FTK_API void setRange(const RangeF&);
        FTK_API void setRange(float, float);
        
        ///@}

        //! \name Increment
        ///@{

        FTK_API float getStep() const;
        FTK_API void setStep(float);

        FTK_API float getLargeStep() const;
        FTK_API void setLargeStep(float);
        
        ///@}

        //! \name Default Value
        ///@{

        FTK_API float getDefaultValue() const;
        FTK_API void setDefaultValue(float);
        
        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<FloatModel>& getModel() const;

        //! Get the display precision.
        FTK_API int getPrecision() const;

        //! Set the display precision.
        FTK_API void setPrecision(int);

        //! Get the font role.
        FTK_API FontRole getFontRole() const;

        //! Set the font role.
        FTK_API void setFontRole(FontRole);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void scrollEvent(ScrollEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        void _textUpdate();

        FTK_PRIVATE();
    };

    //! Floating point reset button.
    class FTK_API_TYPE FloatResetButton : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatResetButton();

    public:
        FTK_API virtual ~FloatResetButton();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FloatResetButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
