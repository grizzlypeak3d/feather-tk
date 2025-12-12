// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>
#include <ftk/UI/IntModel.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Integer value editor and slider.
    class FTK_API_TYPE IntEditSlider : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntEditSlider();

    public:
        FTK_API virtual ~IntEditSlider();

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntEditSlider> create(
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

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
