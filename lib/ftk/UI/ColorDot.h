// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color dot: a small round color swatch for tight rows, subtle where
    //! the square swatch is loud. When editable it opens the color popup.
    class FTK_UI_API_TYPE ColorDot : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ColorDot();

    public:
        FTK_UI_API virtual ~ColorDot();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorDot> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        FTK_UI_API const Color4F& getColor() const;

        //! Set the color.
        FTK_UI_API void setColor(const Color4F&);

        //! Get whether the dot is editable.
        FTK_UI_API bool isEditable() const;

        //! Set whether the dot is editable.
        FTK_UI_API void setEditable(bool);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const Color4F&)>&);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;

    private:
        void _showPopup();

        FTK_PRIVATE();
    };

    ///@}
}
