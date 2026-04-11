// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Assert.h>

#include <iostream>

#include <stdlib.h>

namespace ftk
{
    void ftk_assert(const char* file, int line)
    {
        std::cerr << "ASSERT: " << file << ":" << line << std::endl;
        abort();
    }
}
