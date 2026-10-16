// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FileBrowser.h>

namespace ftk
{
    //! The system's document picker, which is the native file dialog on
    //! iOS: presented over the application's window, and answering
    //! through the callback when the choice is made, on the main thread,
    //! while the application keeps running underneath. The files chosen
    //! are opened in place, wherever they are -- another application's
    //! documents, iCloud, a share -- and stay readable for as long as the
    //! application runs. Picking a directory is the picker for folders;
    //! there is no picker for saving, which the built-in browser does in
    //! the application's own documents.
    void openDocumentPicker(
        const FileBrowserOpenOptions&,
        const std::function<void(const std::vector<Path>&)>&);

    //! Take up again the access to the files chosen in earlier runs: the
    //! grant for a file picked in place ends with the application, and
    //! this is what lets the recent files open without the picker. Called
    //! once, as the file browser system is made.
    void restoreDocumentAccess();
}
