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
        
    //! Double precision floating point value editor and slider.
    class FTK_UI_API_TYPE DoubleEditSlider : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleEditSlider();

    public:
        FTK_UI_API virtual ~DoubleEditSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<DoubleEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<DoubleEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        //! Get the value, which is kept within the range.
        FTK_UI_API double getValue() const;
        //! Set the value. A value outside the range is clamped to it.
        FTK_UI_API void setValue(double);
        //! Set the callback, called with the value whenever it changes,
        //! including from the edit, on every move while the slider is dragged,
        //! from setValue(), and from other widgets that share the model.
        FTK_UI_API void setCallback(const std::function<void(double)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(double, bool)>&);
        
        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within.
        FTK_UI_API const RangeD& getRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeD&);
        //! Set the range from its minimum and maximum.
        FTK_UI_API void setRange(double, double);
        
        ///@}

        //! \name Increment
        ///@{

        //! Get the step: how much the arrow keys, the increment buttons, and
        //! the scroll wheel change the value by.
        FTK_UI_API double getStep() const;
        //! Set the step.
        FTK_UI_API void setStep(double);

        //! Get the large step: how much Page Up and Page Down, and the scroll
        //! wheel with Shift held, change the value by.
        FTK_UI_API double getLargeStep() const;
        //! Set the large step.
        FTK_UI_API void setLargeStep(double);
        
        ///@}

        //! \name Default Value
        ///@{

        //! Get the default value.
        FTK_UI_API double getDefault() const;
        //! Set the default value. A double click on the slider, or Reset in the
        //! slider's popup, resets the value to it.
        FTK_UI_API void setDefault(double);
        
        ///@}

        //! Get the model.
        FTK_UI_API const std::shared_ptr<DoubleModel>& getModel() const;

        //! Get the display precision.
        FTK_UI_API int getPrecision() const;

        //! Set the display precision.
        FTK_UI_API void setPrecision(int);

        //! Get the font.
        FTK_UI_API FontType getFont() const;

        //! Set the font.
        FTK_UI_API void setFont(FontType);


    private:
        void _showPopup();

        FTK_PRIVATE();
    };
        
    ///@}
}
