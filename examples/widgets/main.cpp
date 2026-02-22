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
        auto app = widgets::App::create(context, ftk::convert(argc, argv));
        if (app->getExit() != 0)
            return app->getExit();

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

