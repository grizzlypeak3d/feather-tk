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
//!
//! WARNING: 'value' is not evaluated in Release builds. Never place
//! expressions with side effects inside FTK_ASSERT.
#if defined(FTK_ASSERT)
#undef FTK_ASSERT
#define FTK_ASSERT(value) \
    do { \
        if (!(value)) \
            ftk::ftk_assert(__FILE__, __LINE__); \
    } while(0)
#else // FTK_ASSERT
#define FTK_ASSERT(value) \
    do {} while(0)
#endif // FTK_ASSERT

