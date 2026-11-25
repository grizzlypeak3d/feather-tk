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
        
    //! Integer value editor and shuttle.
    class FTK_UI_API_TYPE IntEditShuttle : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntEditShuttle();

    public:
        FTK_UI_API virtual ~IntEditShuttle();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntEditShuttle> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<IntEditShuttle> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        //! Get the value, which is kept within the range.
        FTK_UI_API int getValue() const;
        //! Set the value. A value outside the range is clamped to it.
        FTK_UI_API void setValue(int);
        //! Set the callback, called with the value whenever it changes,
        //! including from the edit, on every notch while the shuttle is
        //! dragged, from setValue(), and from other widgets that share the
        //! model.
        FTK_UI_API void setCallback(const std::function<void(int)>&);

        //! Set the value callback with a flag for whether the shuttle is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(int, bool)>&);
        
        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within.
        FTK_UI_API const RangeI& getRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeI&);
        //! Set the range from its minimum and maximum.
        FTK_UI_API void setRange(int, int);
        
        ///@}

        //! \name Increment
        ///@{

        //! Get the step: how much the up and down arrow keys, the increment
        //! buttons, the scroll wheel, and each notch of the shuttle change the
        //! value by. With Control held the shuttle moves by a tenth of it, but
        //! at least one.
        FTK_UI_API int getStep() const;
        //! Set the step.
        FTK_UI_API void setStep(int);

        //! Get the large step: how much Page Up and Page Down, and the scroll
        //! wheel or the shuttle with Shift held, change the value by.
        FTK_UI_API int getLargeStep() const;
        //! Set the large step.
        FTK_UI_API void setLargeStep(int);
        
        ///@}

        //! \name Default Value
        ///@{

        //! Get the default value.
        FTK_UI_API int getDefault() const;
        //! Set the default value. The reset button beside the shuttle resets
        //! the value to it.
        FTK_UI_API void setDefault(int);
        
        ///@}

        //! Get the model.
        FTK_UI_API const std::shared_ptr<IntModel>& getModel() const;

        //! Get the font.
        FTK_UI_API FontType getFont() const;

        //! Set the font.
        FTK_UI_API void setFont(FontType);


    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
