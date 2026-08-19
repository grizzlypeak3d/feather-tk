// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/OS.h>

#include <ftk/Core/Memory.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>
#include <shellapi.h>
#include <stdlib.h>
#include <VersionHelpers.h>

#include <array>
#include <codecvt>
#include <cstdlib>
#include <locale>
#include <thread>

namespace ftk
{
    namespace
    {
        enum class Windows
        {
            Unknown,
            _7,
            _8,
            _8_1,
            _10,

            Count
        };

        std::string getLabel(Windows value)
        {
            const std::array<std::string, static_cast<size_t>(Windows::Count)> data =
            {
                "Unknown",
                "Windows 7+",
                "Windows 8+",
                "Windows 8.1+",
                "Windows 10+"
            };
            return data[static_cast<size_t>(value)];
        }

        typedef void (WINAPI* PGNSI)(LPSYSTEM_INFO);

        Windows getWindowsVersion()
        {
            Windows out = Windows::Unknown;
#if _WIN32_WINNT >= 0x0A00
            if (IsWindows10OrGreater())
            {
                out = Windows::_10;
            }
            else
#endif
#if _WIN32_WINNT >= 0x0603
                if (IsWindows8Point1OrGreater())
                {
                    out = Windows::_8_1;
                }
                else
#endif
#if _WIN32_WINNT >= 0x0602
                    if (IsWindows8OrGreater())
                    {
                        out = Windows::_8;
                    }
                    else
#endif
#if _WIN32_WINNT >= 0x0601
                        if (IsWindows7OrGreater())
                        {
                            out = Windows::_7;
                        }
#endif
            return out;
        }

        size_t getRAMSize()
        {
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            return statex.ullTotalPhys;
        }

        std::string getCPUName()
        {
            std::string out;
            // The processor name is published in the registry, which works
            // regardless of architecture (unlike the __cpuid brand string).
            char buf[256];
            DWORD size = sizeof(buf);
            if (ERROR_SUCCESS == RegGetValueA(
                HKEY_LOCAL_MACHINE,
                "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                "ProcessorNameString",
                RRF_RT_REG_SZ,
                nullptr,
                buf,
                &size))
            {
                out = std::string(buf);
            }
            return out;
        }
    }

    SysInfo getSysInfo()
    {
        SysInfo out;
        out.name = getLabel(getWindowsVersion());
        out.cpu = getCPUName();
        out.cores = std::thread::hardware_concurrency();
        out.ram = getRAMSize();
        const auto d = std::lldiv(getRAMSize(), gigabyte);
        out.ramGB = d.quot + (d.rem ? 1 : 0);
        return out;
    }

    bool getEnv(const std::string& name, std::string& value)
    {
        bool out = false;
        size_t size = 0;
        wchar_t* p = 0;
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
        if (0 == _wdupenv_s(&p, &size, utf16.from_bytes(name).c_str()))
        {
            if (p)
            {
                value = utf16.to_bytes(p);
                out = true;
            }
        }
        if (p)
        {
            free(p);
        }
        return out;
    }

    bool setEnv(const std::string& name, const std::string& value)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
        return _wputenv_s(utf16.from_bytes(name).c_str(), utf16.from_bytes(value).c_str()) == 0;
    }

    bool delEnv(const std::string& name)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
        return _wputenv_s(utf16.from_bytes(name).c_str(), utf16.from_bytes(std::string()).c_str()) == 0;
    }

    std::filesystem::path getExePath()
    {
        std::filesystem::path out;
        std::vector<wchar_t> buf(MAX_PATH);
        for (;;)
        {
            const DWORD size = GetModuleFileNameW(
                NULL, buf.data(), static_cast<DWORD>(buf.size()));
            if (0 == size)
            {
                break;
            }
            if (size < buf.size())
            {
                out = std::filesystem::path(std::wstring(buf.data(), size));
                break;
            }
            // Truncated, and it does not say by how much; ask again with
            // more room.
            buf.resize(buf.size() * 2);
        }
        if (!out.empty())
        {
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
        ShellExecute(0, 0, value.c_str(), 0, 0, SW_SHOW);
    }
}