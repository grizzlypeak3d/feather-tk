// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/OS.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Memory.h>

#if defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CFBundle.h>
#include <CoreServices/CoreServices.h>
#endif // __APPLE__

#include <cstdlib>
#include <sstream>
#include <thread>

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
    }
    
    SystemInfo getSystemInfo()
    {
        SystemInfo out;
        out.name = getName();
        out.cores = std::thread::hardware_concurrency();
        out.ram = getRAMSize();
        const auto d = std::lldiv(getRAMSize(), gigabyte);
        out.ramGB = d.quot + (d.rem ? 1 : 0);
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
