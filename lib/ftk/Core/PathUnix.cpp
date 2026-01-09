// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Path.h>

#if defined(__APPLE__)
#include "PathMacOSPrivate.h"
#endif // __APPLE__

#include <cstdlib>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

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

    std::filesystem::path createTmpDir()
    {
        std::string path;
        char* env = nullptr;
        if ((env = getenv("TEMP"))) path = env;
        else if ((env = getenv("TMP"))) path = env;
        else if ((env = getenv("TMPDIR"))) path = env;
        else
        {
            for (const auto& i : { "/tmp", "/var/tmp", "/usr/tmp" })
            {
                struct stat buffer;
                if (0 == stat(i, &buffer))
                {
                    path = i;
                    break;
                }
            }
        }

        path = path + "/XXXXXX";
        const size_t size = path.size();
        std::vector<char> buf(size + 1);
        memcpy(buf.data(), path.c_str(), size);
        buf[size] = 0;
        return std::filesystem::u8path(mkdtemp(buf.data()));
    }
}
