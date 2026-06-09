// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

#include <ftk/UI/FileBrowser.h>

FTK_MAIN()
{
    try
    {
        // Create the context and application.
        auto context = ftk::Context::create();
        auto args = ftk::convert(argc, argv);
        auto app = widgets::App::create(context, args);
        if (app->hasCmdLineHelp())
            return 0;

        // Initialize the file browser.
        auto fileBrowserSystem = context->getSystem<ftk::FileBrowserSystem>();
        fileBrowserSystem->setNativeFileDialog(false);

        // Run the application.
        app->run();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
