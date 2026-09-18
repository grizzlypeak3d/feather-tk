// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IconSystem.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Version.h>

#include <iostream>

// Creates a context and looks up an icon, which loads the libraries the
// wheel carries -- zlib, libpng, FreeType, lunasvg -- as well as linking
// them.
int main()
{
    std::cout << "feather-tk " << FTK_VERSION_FULL << std::endl;
    auto context = ftk::Context::create();
    auto iconSystem = ftk::IconSystem::create(context);
    return iconSystem->getNames().empty() ? 1 : 0;
}
