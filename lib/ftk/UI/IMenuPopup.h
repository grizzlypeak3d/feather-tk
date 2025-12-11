// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IPopup.h>

namespace ftk
{
    class IWindow;
    class ScrollWidget;

    //! \name Menu Widgets
    ///@{

    //! Menu popup type.
    enum class FTK_API_TYPE MenuPopup
    {
        Menu,
        SubMenu
    };

    //! Base class for popup menus.
    class FTK_API_TYPE IMenuPopup : public IPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        IMenuPopup();

    public:
        virtual ~IMenuPopup() = 0;

        //! Open the popup.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        virtual void open(
            const std::shared_ptr<IWindow>&,
            const Box2I& buttonGeometry);

        //! Get whether the popup is open.
        bool isOpen() const;

        //! Set the close callback.
        void setCloseCallback(const std::function<void(void)>&);

        //! Set the popup type.
        void setPopup(MenuPopup);

        //! Set the popup color role.
        void setPopupRole(ColorRole);

        //! Set the popup widget.
        void setWidget(const std::shared_ptr<IWidget>&);

        void close() override;

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    protected:
        const std::shared_ptr<ScrollWidget>& _getScrollWidget() const;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
