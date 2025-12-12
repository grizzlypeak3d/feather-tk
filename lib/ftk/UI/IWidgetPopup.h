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
        FTK_API virtual ~IWidgetPopup() = 0;

        //! Open the popup.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        FTK_API void open(
            const std::shared_ptr<IWindow>&,
            const Box2I& buttonGeometry);

        //! Get whether the popup is open.
        FTK_API bool isOpen() const;

        //! Close the popup.
        FTK_API void close() override;

        //! Set the close callback.
        FTK_API void setCloseCallback(const std::function<void(void)>&);

        //! Get the widget.
        FTK_API const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        FTK_API void setWidget(const std::shared_ptr<IWidget>&);

        //! Get the popup color role.
        FTK_API ColorRole getPopupRole() const;

        //! Set the popup color role.
        FTK_API void setPopupRole(ColorRole);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
