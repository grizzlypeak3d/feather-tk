// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <stdexcept>

namespace ftk
{
    //! \name Errors
    ///@{
        
    //! Parse error.
    class FTK_API_TYPE ParseError : public std::invalid_argument
    {
    public:
        FTK_API ParseError();
    };
        
#if defined(_WINDOWS)
    //! Get an error string from a Windows system call.
    FTK_API std::string getLastError();
#endif // _WINDOWS

    ///@}
}
