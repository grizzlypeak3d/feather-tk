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
        
    //! Double precision floating point value editor and shuttle.
    class FTK_UI_API_TYPE DoubleEditShuttle : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleEditShuttle();

    public:
        FTK_UI_API virtual ~DoubleEditShuttle();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<DoubleEditShuttle> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<DoubleEditShuttle> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_UI_API double getValue() const;
        FTK_UI_API void setValue(double);
        FTK_UI_API void setCallback(const std::function<void(double)>&);

        //! Set the value callback with a flag for whether the shuttle is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(double, bool)>&);
        
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
        FTK_PRIVATE();
    };
        
    ///@}
}
