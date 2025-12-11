// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IPopup.h>

namespace ftk
{
    class IWindow;

    //! \name Popup Widgets
    ///@{

    //! Base class for popup widgets.
    class FTK_API_TYPE IWidgetPopup : public IPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        IWidgetPopup();

    public:
        virtual ~IWidgetPopup() = 0;

        //! Open the popup.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        void open(
            const std::shared_ptr<IWindow>&,
            const Box2I& buttonGeometry);

        //! Get whether the popup is open.
        bool isOpen() const;

        //! Close the popup.
        void close() override;

        //! Set the close callback.
        void setCloseCallback(const std::function<void(void)>&);

        //! Get the widget.
        const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        void setWidget(const std::shared_ptr<IWidget>&);

        //! Get the popup color role.
        ColorRole getPopupRole() const;

        //! Set the popup color role.
        void setPopupRole(ColorRole);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
