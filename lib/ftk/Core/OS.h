// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <filesystem>
#include <string>
#include <vector>

namespace ftk
{
    //! \name Operating System
    ///@{

    //! Operating system information.
    struct FTK_CORE_API_TYPE SysInfo
    {
        std::string name;
        std::string cpu;
        size_t      cores = 0;
        size_t      ram   = 0;
        size_t      ramGB = 0;

        //! The encoding the platform converts narrow strings through: the
        //! ANSI code page on Windows, the locale's character set elsewhere.
        //! Worth having in the log because a file name the encoding cannot
        //! represent is why a path can open on one machine and not another,
        //! and nothing else on the machine says which encoding it is.
        std::string codePage;

        //! The user's locale, which is what selects the code page above.
        std::string locale;
    };

    //! Get operating system information.
    FTK_CORE_API SysInfo getSysInfo();

    //! Environment variable list separators.
    const std::vector<char> envListSeparators = { ':', ';' };

    //! Environment variable list separator.
#if defined(_WINDOWS)
    constexpr char envListSeparator = ';';
#else // _WINDOWS
    constexpr char envListSeparator = ':';
#endif // _WINDOWS

    //! Get an environment variable. Throws std::exception on failure.
    FTK_CORE_API bool getEnv(const std::string& name, std::string&);

    //! Get an environment variable and convert it to an integer; an empty
    //! variable gives zero. Throws std::exception on failure.
    FTK_CORE_API bool getEnv(const std::string& name, int& value);

    //! Get an environment variable and convert it to a list of strings.
    //! Throws std::exception on failure.
    FTK_CORE_API bool getEnv(const std::string& name, std::vector<std::string>&);

    //! Set an environment variable. Throws std::exception on failure.
    FTK_CORE_API bool setEnv(const std::string& name, const std::string&);

    //! Delete an environment variable. Throws std::exception on failure.
    FTK_CORE_API bool delEnv(const std::string& name);

    //! Open the given URL in the default browser. Throws std::exception on
    //! failure.
    //! Get the path of the running executable.
    //!
    //! Not argv[0], which is however the program was invoked and need not be
    //! a path at all. This is what an application looks for the things
    //! installed beside it from.
    FTK_CORE_API std::filesystem::path getExePath();

    FTK_CORE_API void openURL(const std::string&);

    ///@}
}
