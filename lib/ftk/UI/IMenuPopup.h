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
        FTK_API virtual ~IMenuPopup() = 0;

        //! Open the popup.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        FTK_API virtual void open(
            const std::shared_ptr<IWindow>&,
            const Box2I& buttonGeometry);

        //! Get whether the popup is open.
        FTK_API bool isOpen() const;

        //! Set the close callback.
        FTK_API void setCloseCallback(const std::function<void(void)>&);

        //! Set the popup type.
        FTK_API void setPopup(MenuPopup);

        //! Set the popup color role.
        FTK_API void setPopupRole(ColorRole);

        //! Set the popup widget.
        FTK_API void setWidget(const std::shared_ptr<IWidget>&);

        FTK_API void close() override;

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    protected:
        const std::shared_ptr<ScrollWidget>& _getScrollWidget() const;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
