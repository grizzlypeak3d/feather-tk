// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <string>
#include <vector>

namespace ftk
{
    //! \name Operating System
    ///@{

    //! Operating system information.
    struct FTK_API_TYPE SysInfo
    {
        std::string name;
        size_t      cores = 0;
        size_t      ram   = 0;
        size_t      ramGB = 0;
    };

    //! Get operating system information.
    FTK_API SysInfo getSysInfo();

    //! Environment variable list separators.
    const std::vector<char> envListSeparators = { ':', ';' };

    //! Environment variable list separator.
#if defined(_WINDOWS)
    constexpr char envListSeparator = ';';
#else // _WINDOWS
    constexpr char envListSeparator = ':';
#endif // _WINDOWS

    //! Get an environment variable.
    //! 
    //! Throws:
    //! - std::exception
    FTK_API bool getEnv(const std::string& name, std::string&);

    //! Get an environment variable and convert it to an integer. If the
    //! variable is empty then zero is returned.
    //!
    //! Throws:
    //! - std::exception
    FTK_API bool getEnv(const std::string& name, int& value);

    //! Get an environment variable and convert it to a list of strings.
    //!
    //! Throws:
    //! - std::exception
    FTK_API bool getEnv(const std::string& name, std::vector<std::string>&);

    //! Set an environment variable.
    //! 
    //! Throws:
    //! - std::exception
    FTK_API bool setEnv(const std::string& name, const std::string&);

    //! Delete an environment variable.
    //! 
    //! Throws:
    //! - std::exception
    FTK_API bool delEnv(const std::string& name);

    //! Open the given URL in the default browser.
    //!
    //! Throws:
    //! - std::exception
    FTK_API void openURL(const std::string&);

    ///@}
}
