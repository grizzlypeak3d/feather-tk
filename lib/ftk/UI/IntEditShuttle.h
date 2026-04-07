// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>
#include <ftk/UI/IntModel.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{
        
    //! Integer value editor and shuttle.
    class FTK_API_TYPE IntEditShuttle : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntEditShuttle();

    public:
        FTK_API virtual ~IntEditShuttle();

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntEditShuttle> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<IntEditShuttle> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_API int getValue() const;
        FTK_API void setValue(int);
        FTK_API void setCallback(const std::function<void(int)>&);

        //! Set the value callback with a flag for whether the shuttle is presssed.
        FTK_API void setPressedCallback(const std::function<void(int, bool)>&);
        
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

        FTK_API int getDefault() const;
        FTK_API void setDefault(int);
        
        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<IntModel>& getModel() const;

        //! Get the font.
        FTK_API FontType getFont() const;

        //! Set the font.
        FTK_API void setFont(FontType);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
