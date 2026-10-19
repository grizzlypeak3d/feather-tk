// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IPopup.h>

namespace ftk
{
    class IWindow;

    //! \name Dialogs
    ///@{

    //! How a dialog is sized in the window it opens in. Never larger than
    //! the window, whichever it is.
    enum class FTK_UI_API_TYPE DialogSize
    {
        //! As big as its content asks for, or across the window in a
        //! direction the content's stretch expands in. For messages,
        //! questions and settings.
        Fit,

        //! A column of text: as wide as the text width, or as the content
        //! if that is wider, and the height of the window. For agreements,
        //! notes and help, which are read rather than glanced at, and whose
        //! lines are hard to follow when they run across a wide window.
        Text,

        //! The window, less the dialog margin. For lists and reports, which
        //! use whatever room there is.
        Full,

        Count,
        First = Fit
    };
    FTK_ENUM(FTK_UI_API, DialogSize);

    //! Base class for dialog widgets.
    class FTK_UI_API_TYPE IDialog : public IPopup
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_UI_API IDialog();

    public:
        FTK_UI_API virtual ~IDialog() = 0;

        //! Open the dialog.
        //!
        //! \todo Should setVisible() be used instead of open() and
        //! close()?
        FTK_UI_API void open(const std::shared_ptr<IWindow>&);

        //! Get whether the dialog is open.
        FTK_UI_API bool isOpen() const;

        //! Get the title.
        FTK_UI_API const std::string& getTitle() const;

        //! Set the title, shown in a bar above the dialog. An empty title,
        //! the default, has no bar. Titles belong to dialogs: a widget used
        //! outside one does not carry a title bar around with it.
        FTK_UI_API void setTitle(const std::string&);

        //! Set the close callback.
        FTK_UI_API void setCloseCallback(const std::function<void(void)>&);

        //! Get how the dialog is sized.
        FTK_UI_API DialogSize getDialogSize() const;

        //! Get the width of a DialogSize::Text dialog, in characters.
        FTK_UI_API int getTextWidth() const;

        //! Set how the dialog is sized, and for DialogSize::Text, how many
        //! characters wide it is, measured as a label's wrap width is. Fit,
        //! the default, is what the content's stretch says. Text and Full
        //! decide the size themselves, whatever the content's stretch, so
        //! their content should fill the room it is given: labels in it
        //! wrapping to fit, a scroll widget expanding.
        FTK_UI_API void setDialogSize(DialogSize, int textWidth = 100);

        //! Get the key focus widget.
        FTK_UI_API virtual std::shared_ptr<IWidget> getKeyFocus() const;

        FTK_UI_API void close() override;

        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _takeKeyFocus();

        FTK_PRIVATE();
    };

    ///@}
}
