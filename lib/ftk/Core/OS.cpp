// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/OS.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Path.h>
#include <ftk/Core/String.h>

#include <chrono>
#include <ctime>

namespace ftk
{
    bool getEnv(const std::string& name, int& out)
    {
        std::string value;
        if (getEnv(name, value))
        {
            out = !value.empty() ? std::stoi(value) : 0;
            return true;
        }
        return false;
    }

    bool getEnv(const std::string& name, std::vector<std::string>& out)
    {
        std::string value;
        if (getEnv(name, value))
        {
            out = split(value, envListSeparator);
            return true;
        }
        return false;
    }

    std::string getLibraryInfo(const void* address)
    {
        std::string out;
        const std::filesystem::path path = getLibraryPath(address);
        if (!path.empty())
        {
            std::error_code ec;
            const auto fileTime = std::filesystem::last_write_time(path, ec);
            std::string modified;
            if (!ec)
            {
                const auto systemTime =
                    std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        fileTime -
                        std::filesystem::file_time_type::clock::now() +
                        std::chrono::system_clock::now());
                const std::time_t t = std::chrono::system_clock::to_time_t(systemTime);
                char buf[32];
                if (std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t)))
                {
                    modified = buf;
                }
            }
            out = Format("Library: \"{0}\", modified {1}").
                arg(fromFileSystem(path)).
                arg(modified);
        }
        return out;
    }
}
