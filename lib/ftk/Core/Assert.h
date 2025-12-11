// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

//! Project namespace
namespace ftk
{
    //! \name Assert
    ///@{
        
    //! Assert (for convenience use the FTK_ASSERT macro).
    FTK_API void ftk_assert(const char* file, int line);

    ///@}
}

//! Assert macro.
#if defined(FTK_ASSERT)
#undef FTK_ASSERT
#define FTK_ASSERT(value) \
    if (!(value)) \
        ftk::ftk_assert(__FILE__, __LINE__)
#else // FTK_ASSERT
#define FTK_ASSERT(value)
#endif // FTK_ASSERT

