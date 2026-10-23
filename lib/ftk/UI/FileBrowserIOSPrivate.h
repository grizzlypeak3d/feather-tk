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
    //! application runs. Picking a directory is the picker for folders.
    void openDocumentPicker(
        const FileBrowserOpenOptions&,
        const std::function<void(const std::vector<Path>&)>&);

    //! The picker for saving, which is the picker for exporting: a file
    //! written already is moved to the place the user chooses, and the
    //! callback is given that place, with the access to write it again.
    //! Cancelled, the file is left where it was and the second callback
    //! runs instead.
    void exportDocument(
        const Path& file,
        const std::function<void(const std::vector<Path>&)>& callback,
        const std::function<void(void)>& cancelled);

    //! Take up again the access to the files chosen in earlier runs: the
    //! grant for a file picked in place ends with the application, and
    //! this is what lets the recent files open without the picker. Called
    //! once, as the file browser system is made.
    void restoreDocumentAccess();
}
