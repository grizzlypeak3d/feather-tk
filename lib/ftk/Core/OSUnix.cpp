// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/OS.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Memory.h>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CFBundle.h>
#include <CoreServices/CoreServices.h>
#endif // __APPLE__

#include <clocale>
#include <cstdlib>
#include <fstream>
#include <langinfo.h>
#include <sstream>
#include <thread>
#include <vector>

#include <sys/ioctl.h>
#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#elif defined(__FreeBSD__)
#include <sys/sysctl.h>
#else // __APPLE__
#include <sys/sysinfo.h>
#endif // __APPLE__
#include <sys/utsname.h>
#include <pwd.h>
#include <unistd.h>

namespace ftk
{
    namespace
    {
        std::string getName()
        {
            std::string out;
            ::utsname info;
            uname(&info);
            std::stringstream s;
            s << info.sysname << " " << info.release << " " << info.machine;
            out = s.str();
            return out;
        }
        
        std::string getCPUName()
        {
            std::string out;
#if defined(__APPLE__)
            size_t len = 0;
            if (0 == sysctlbyname("machdep.cpu.brand_string", nullptr, &len, nullptr, 0) &&
                len > 0)
            {
                std::vector<char> buf(len);
                if (0 == sysctlbyname("machdep.cpu.brand_string", buf.data(), &len, nullptr, 0))
                {
                    out = std::string(buf.data());
                }
            }
#else // __APPLE__
            // Use the first "model name" line in /proc/cpuinfo.
            std::ifstream is("/proc/cpuinfo");
            std::string line;
            while (std::getline(is, line))
            {
                const auto i = line.find("model name");
                if (0 == i)
                {
                    const auto j = line.find(':');
                    if (j != std::string::npos)
                    {
                        out = line.substr(j + 1);
                        // Trim leading whitespace.
                        const auto k = out.find_first_not_of(" \t");
                        out = (k != std::string::npos) ? out.substr(k) : std::string();
                    }
                    break;
                }
            }
#endif // __APPLE__
            return out;
        }

        size_t getRAMSize()
        {
            size_t out = 0;
#if defined(__APPLE__)
            int name[2] = { CTL_HW, HW_MEMSIZE };
            u_int namelen = sizeof(name) / sizeof(name[0]);
            uint64_t size = 0;
            size_t len = sizeof(size);
            if (0 == sysctl(name, namelen, &size, &len, NULL, 0))
            {
                out = static_cast<size_t>(size);
            }
#elif defined(__FreeBSD__)
            uint64_t size = 0;
            size_t len = sizeof(size);
            if (0 == sysctlbyname("hw.physmem", &size, &len, NULL, 0))
            {
                out = static_cast<size_t>(size);
            }
#else // __APPLE__
            struct sysinfo info;
            if (0 == sysinfo(&info))
            {
                out = info.totalram;
            }
#endif // __APPLE__
            return out;
        }

        std::string getCodePage()
        {
            // Paths here are bytes and are treated as UTF-8 throughout, so
            // this cannot go wrong the way the Windows code page can. It is
            // reported anyway so that logs from the two platforms answer the
            // same questions.
            std::string out;
            if (const char* p = ::nl_langinfo(CODESET))
            {
                out = p;
            }
            return out;
        }

        std::string getLocale()
        {
            std::string out;
            if (const char* p = std::setlocale(LC_CTYPE, nullptr))
            {
                out = p;
            }
            return out;
        }
    }

    SysInfo getSysInfo()
    {
        SysInfo out;
        out.name = getName();
        out.cpu = getCPUName();
        out.cores = std::thread::hardware_concurrency();
        out.ram = getRAMSize();
        const auto d = std::lldiv(getRAMSize(), gigabyte);
        out.ramGB = d.quot + (d.rem ? 1 : 0);
        out.codePage = getCodePage();
        out.locale = getLocale();
        return out;
    }
            
    bool getEnv(const std::string& name, std::string& out)
    {
        if (const char* p = ::getenv(name.c_str()))
        {
            out = std::string(p);
            return true;
        }
        return false;
    }

    bool setEnv(const std::string& name, const std::string& value)
    {
        return ::setenv(name.c_str(), value.c_str(), 1) == 0;
    }

    bool delEnv(const std::string& name)
    {
        return ::unsetenv(name.c_str()) == 0;
    }

    std::filesystem::path getExePath()
    {
        std::filesystem::path out;
#if defined(__APPLE__)
        // Asked twice: the first call says how much room the path needs.
        uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        std::vector<char> buf(size + 1, 0);
        if (0 == _NSGetExecutablePath(buf.data(), &size))
        {
            out = std::filesystem::path(buf.data());
        }
#else // __APPLE__
        std::error_code ec;
        const std::filesystem::path link =
            std::filesystem::read_symlink("/proc/self/exe", ec);
        if (!ec)
        {
            out = link;
        }
#endif // __APPLE__
        if (!out.empty())
        {
            // The symbolic links along the way are followed, so that an
            // application started through one still finds what was installed
            // beside it.
            std::error_code ec;
            const std::filesystem::path canonical =
                std::filesystem::canonical(out, ec);
            if (!ec)
            {
                out = canonical;
            }
        }
        return out;
    }

    void openURL(const std::string& value)
    {
#if defined(__APPLE__)
        CFURLRef url = CFURLCreateWithBytes(
            NULL,
            (UInt8*)value.c_str(),
            value.size(),
            kCFStringEncodingASCII,
            NULL);
        LSOpenCFURLRef(url, 0);
        CFRelease(url);
#else // __APPLE__
        std::stringstream ss;
        ss << "xdg-open" << " " << value;
        if (system(ss.str().c_str()) != 0)
        {
            throw std::runtime_error(Format("Cannot open URL: {0}").arg(value));
        }
#endif // __APPLE__
    }
}
