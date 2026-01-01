// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IPopup.h>

namespace ftk
{
    class IWindow;

    //! \name Dialogs
    ///@{

    //! Base class for dialog widgets.
    class FTK_API_TYPE IDialog : public IPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        IDialog();

    public:
        FTK_API virtual ~IDialog() = 0;

        //! Open the dialog.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        FTK_API void open(const std::shared_ptr<IWindow>&);

        //! Get whether the dialog is open.
        FTK_API bool isOpen() const;

        //! Set the close callback.
        FTK_API void setCloseCallback(const std::function<void(void)>&);

        //! Get the key focus widget.
        FTK_API virtual std::shared_ptr<IWidget> getKeyFocus() const;

        FTK_API void close() override;

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void scrollEvent(ScrollEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
