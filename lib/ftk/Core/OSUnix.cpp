// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/OS.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Memory.h>
#include <ftk/Core/Path.h>

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
#elif defined(__EMSCRIPTEN__)
#else // __APPLE__
#include <sys/sysinfo.h>
#endif // __APPLE__
#include <sys/utsname.h>
#include <sys/wait.h>
#include <cstring>
#include <pwd.h>
#include <dlfcn.h>
#include <spawn.h>
#include <unistd.h>

extern char** environ;

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
#elif defined(__EMSCRIPTEN__)
            // The browser does not say. Report what WebAssembly can
            // address rather than nothing.
            out = 4ULL * 1024 * 1024 * 1024;
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
#if !defined(__APPLE__)
        // What the session says it is: "labwc:wlroots", "ubuntu:GNOME".
        std::string desktop;
        if (!getEnv("XDG_CURRENT_DESKTOP", desktop) || desktop.empty())
        {
            getEnv("DESKTOP_SESSION", desktop);
        }
        std::string sessionType;
        getEnv("XDG_SESSION_TYPE", sessionType);
        if (!desktop.empty() && !sessionType.empty())
        {
            out.desktop = desktop + " (" + sessionType + ")";
        }
        else
        {
            out.desktop = !desktop.empty() ? desktop : sessionType;
        }
#endif // __APPLE__
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
        // NULL for a string that is not a URL, which CFRelease would crash
        // on.
        if (!url)
        {
            throw std::runtime_error(Format("Cannot open URL: {0}").arg(value));
        }
        LSOpenCFURLRef(url, 0);
        CFRelease(url);
#elif defined(__EMSCRIPTEN__)
        // A browser has no xdg-open, and system() could never run one:
        // window.open() would be the way. Not done yet, so this says so, as
        // it always did.
        throw std::runtime_error(Format("Cannot open URL: {0}").arg(value));
#else // __APPLE__
        // xdg-open itself, with the URL as its one argument, rather than a
        // command line handed to a shell: unquoted there, a space split the
        // URL in two, "&" ran the start of it in the background and dropped
        // the rest -- a mail link's body -- and "?" or "*" could match file
        // names. Nothing is parsed now, so every URL arrives as it was.
        const char* argv[] = { "xdg-open", value.c_str(), nullptr };
        pid_t pid = 0;
        const int result = posix_spawnp(
            &pid,
            "xdg-open",
            nullptr,
            nullptr,
            const_cast<char* const*>(argv),
            environ);
        if (result != 0)
        {
            throw std::runtime_error(Format("Cannot open URL: {0}: {1}").
                arg(value).
                arg(std::strerror(result)));
        }
        // Waited for, as system() did: xdg-open hands the URL to the desktop
        // and returns, and waiting is what keeps it from being left a
        // zombie.
        int status = 0;
        if (waitpid(pid, &status, 0) < 0 ||
            !WIFEXITED(status) ||
            WEXITSTATUS(status) != 0)
        {
            throw std::runtime_error(Format("Cannot open URL: {0}").arg(value));
        }
#endif // __APPLE__
    }

    std::filesystem::path getLibraryPath(const void* address)
    {
        std::filesystem::path out;
        Dl_info info;
        if (dladdr(address, &info) && info.dli_fname)
        {
            out = toFileSystem(info.dli_fname);
        }
        return out;
    }
}
