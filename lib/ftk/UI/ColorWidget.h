// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    enum class FTK_API_TYPE ColorWidgetMode
    {
        RGB,
        HSV,

        Count,
        First = RGB
    };
    FTK_ENUM(ColorWidgetMode);

    //! Color widget.
    //!
    //! \todo Add support for displaying pixel types like U8, U16, etc.?
    class FTK_API_TYPE ColorWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ColorWidget();

    public:
        FTK_API virtual ~ColorWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ColorWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        FTK_API const Color4F& getColor() const;

        //! Set the color.
        FTK_API void setColor(const Color4F&);

        //! Set the color callback.
        FTK_API void setCallback(const std::function<void(const Color4F&)>&);

        //! Get the widget mode.
        FTK_API ColorWidgetMode getMode() const;

        //! Set the widget mode.
        FTK_API void setMode(ColorWidgetMode);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        void _modeUpdate();
        void _colorUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
