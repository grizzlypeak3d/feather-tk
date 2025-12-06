// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Path.h>

#if defined(__APPLE__)
#include "PathMacOSPrivate.h"
#endif // __APPLE__

#include <cstdlib>

namespace ftk
{
    std::vector<std::filesystem::path> getDrives()
    {
        std::vector<std::filesystem::path> out;
        out.push_back(std::filesystem::path("/"));
#if defined(__APPLE__)
        for (const auto& i : std::filesystem::directory_iterator("/Volumes"))
        {
            out.push_back(i.path());
        }
#else // __APPLE__
        for (const auto& i : std::filesystem::directory_iterator("/mnt"))
        {
            out.push_back(i.path());
        }
#endif // __APPLE__
        return out;
    }

    std::filesystem::path getUserPath(UserPath value)
    {
        std::filesystem::path out;
#if defined(__APPLE__)
        switch (value)
        {
        case UserPath::Home:      out = getHomePath(); break;
        case UserPath::Desktop:   out = getDesktopPath(); break;
        case UserPath::Documents: out = getDocsPath(); break;
        case UserPath::Downloads: out = getDownloadsPath(); break;
        default: break;
        }
#else // __APPLE__
        const std::filesystem::path home = std::getenv("HOME");
        switch (value)
        {
        case UserPath::Home:      out = home; break;
        case UserPath::Desktop:   out = home / "Desktop";   break;
        case UserPath::Documents: out = home / "Documents"; break;
        case UserPath::Downloads: out = home / "Downloads"; break;
        default: break;
        }
#endif // __APPLE__
        return out;
    }
}
