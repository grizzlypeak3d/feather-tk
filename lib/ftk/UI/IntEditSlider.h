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
        
    //! Integer value editor and slider.
    class FTK_UI_API_TYPE IntEditSlider : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntEditSlider();

    public:
        FTK_UI_API virtual ~IntEditSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_UI_API int getValue() const;
        FTK_UI_API void setValue(int);
        FTK_UI_API void setCallback(const std::function<void(int)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(int, bool)>&);
        
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

        //! Get the model.
        FTK_UI_API const std::shared_ptr<IntModel>& getModel() const;

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
