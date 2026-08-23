// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color swatch widget.
    class FTK_UI_API_TYPE ColorSwatch : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ColorSwatch();

    public:
        FTK_UI_API virtual ~ColorSwatch();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorSwatch> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        FTK_UI_API const Color4F& getColor() const;

        //! Set the color.
        FTK_UI_API void setColor(const Color4F&);

        //! Get whether the color is editable.
        FTK_UI_API bool isEditable() const;

        //! Set whether the color is editable.
        FTK_UI_API void setEditable(bool);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const Color4F&)>&);

        //! Set the callback with a flag for whether the widget is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(const Color4F&, bool)>&);

        //! Get whether a border is shown.
        FTK_UI_API bool hasBorder() const;

        //! Set whether a border is shown.
        FTK_UI_API void setBorder(bool);

        //! Get the size role.
        FTK_UI_API SizeRole getSizeRole() const;

        //! Set the size role.
        FTK_UI_API void setSizeRole(SizeRole);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;

    private:
        void _showPopup();

        FTK_PRIVATE();
    };

    ///@}
}
