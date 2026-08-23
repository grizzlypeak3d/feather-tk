// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IPopup.h>

namespace ftk
{
    class IWindow;
    class ScrollWidget;

    //! \name Menu Widgets
    ///@{

    //! Menu popup type.
    enum class FTK_UI_API_TYPE MenuPopup
    {
        Menu,
        SubMenu
    };

    //! Base class for popup menus.
    class FTK_UI_API_TYPE IMenuPopup : public IPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        IMenuPopup();

    public:
        FTK_UI_API virtual ~IMenuPopup() = 0;

        //! Open the popup.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        FTK_UI_API virtual void open(
            const std::shared_ptr<IWindow>&,
            const Box2I& buttonGeometry);

        //! Open the popup at a position, for example at the cursor for a
        //! context menu.
        FTK_UI_API void open(
            const std::shared_ptr<IWindow>&,
            const V2I& pos);

        //! Get whether the popup is open.
        FTK_UI_API bool isOpen() const;

        //! Set the close callback.
        FTK_UI_API void setCloseCallback(const std::function<void(void)>&);

        //! Set the popup type.
        FTK_UI_API void setPopup(MenuPopup);

        //! Set the popup color role.
        FTK_UI_API void setPopupRole(ColorRole);

        //! Set the popup widget.
        FTK_UI_API void setWidget(const std::shared_ptr<IWidget>&);

        FTK_UI_API void close() override;

        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void scrollEvent(ScrollEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    protected:
        const std::shared_ptr<ScrollWidget>& _getScrollWidget() const;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
