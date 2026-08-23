// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IPopup.h>

namespace ftk
{
    class IWindow;

    //! \name Popup Widgets
    ///@{

    //! Base class for popup widgets.
    class FTK_UI_API_TYPE IWidgetPopup : public IPopup
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_UI_API IWidgetPopup();

    public:
        FTK_UI_API virtual ~IWidgetPopup() = 0;

        //! Open the popup.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        FTK_UI_API virtual void open(
            const std::shared_ptr<IWindow>&,
            const Box2I& buttonGeometry,
            const std::optional<Box2I>& widgetGeometry = std::optional<Box2I>());

        //! Get whether the popup is open.
        FTK_UI_API bool isOpen() const;

        //! Close the popup.
        FTK_UI_API void close() override;

        //! Set the close callback.
        FTK_UI_API void setCloseCallback(const std::function<void(void)>&);

        //! Get the widget.
        FTK_UI_API const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        FTK_UI_API void setWidget(const std::shared_ptr<IWidget>&);

        //! Get the popup color role.
        FTK_UI_API ColorRole getPopupRole() const;

        //! Set the popup color role.
        FTK_UI_API void setPopupRole(ColorRole);

        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void scrollEvent(ScrollEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
