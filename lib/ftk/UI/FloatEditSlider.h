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
        
    //! Floating point value editor and slider.
    class FTK_UI_API_TYPE FloatEditSlider : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatEditSlider();

    public:
        FTK_UI_API virtual ~FloatEditSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_UI_API float getValue() const;
        FTK_UI_API void setValue(float);
        FTK_UI_API void setCallback(const std::function<void(float)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(float, bool)>&);
        
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

        //! Get the model.
        FTK_UI_API const std::shared_ptr<FloatModel>& getModel() const;

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
