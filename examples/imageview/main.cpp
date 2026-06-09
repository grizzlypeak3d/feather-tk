// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

FTK_MAIN()
{
    try
    {
        auto context = ftk::Context::create();
        auto app = imageview::App::create(context, ftk::convert(argc, argv));
        if (app->hasCmdLineHelp())
            return 0;
        app->run();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
