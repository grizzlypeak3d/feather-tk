// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

FTK_MAIN()
{
    try
    {
        auto context = ftk::Context::create();
        auto app = objview::App::create(context, ftk::convert(argc, argv));
        return app->run();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    return 1;
}
