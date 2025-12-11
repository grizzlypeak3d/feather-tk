// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color swatch widget.
    class FTK_API_TYPE ColorSwatch : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ColorSwatch();

    public:
        virtual ~ColorSwatch();

        //! Create a new widget.
        static std::shared_ptr<ColorSwatch> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        const Color4F& getColor() const;

        //! Set the color.
        void setColor(const Color4F&);

        //! Set the pressed callback.
        void setPressedCallback(const std::function<void(void)>&);

        //! Get whether the color is editable.
        bool isEditable() const;

        //! Set whether the color is editable.
        void setEditable(bool);

        //! Set the color callback.
        void setColorCallback(const std::function<void(const Color4F&)>&);

        //! Get the size role.
        SizeRole getSizeRole() const;

        //! Set the size role.
        void setSizeRole(SizeRole);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;

    private:
        void _showPopup();

        FTK_PRIVATE();
    };

    ///@}
}
