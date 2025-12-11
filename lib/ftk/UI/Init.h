// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <memory>
#include <string>

namespace ftk
{
    class Context;

    //! Initialize the UI library.
    void FTK_API uiInit(const std::shared_ptr<Context>&);
}

