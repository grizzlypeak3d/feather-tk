// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Path.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>
#include <combaseapi.h>
#include <Shlobj.h>
#include <shellapi.h>

#include <string>

namespace ftk
{
    std::vector<std::filesystem::path> getDrives()
    {
        std::vector<std::filesystem::path> out;
        if (DWORD result = GetLogicalDriveStringsW(0, NULL))
        {
            std::vector<WCHAR> buf(result);
            result = GetLogicalDriveStringsW(result, buf.data());
            if (result)
            {
                for (WCHAR* p = buf.data(), *end = buf.data() + result; p < end && *p; ++p)
                {
                    WCHAR* p2 = p;
                    for (; p2 < end && *p2; ++p2)
                        ;
                    // The wide string goes straight into the path. Converting
                    // it to UTF-8 first would be undone on the way in: a path
                    // built from a narrow string reads it as the ANSI code
                    // page, not as UTF-8.
                    out.push_back(std::filesystem::path(std::wstring(p, p2 - p)));
                    p = p2;
                }
            }
        }
        return out;
    }

    std::filesystem::path getUserPath(UserPath value)
    {
        std::filesystem::path out;
        KNOWNFOLDERID id;
        memset(&id, 0, sizeof(KNOWNFOLDERID));
        switch (value)
        {
        case UserPath::Home:      id = FOLDERID_Profile;   break;
        case UserPath::Desktop:   id = FOLDERID_Desktop;   break;
        case UserPath::Documents: id = FOLDERID_Documents; break;
        case UserPath::Downloads: id = FOLDERID_Downloads; break;
        default: break;
        }
        wchar_t* path = nullptr;
        HRESULT result = SHGetKnownFolderPath(id, 0, NULL, &path);
        if (S_OK == result && path)
        {
            // Wide throughout: a user whose account name is not representable
            // in the ANSI code page would otherwise get a mangled home
            // directory here, and with it the wrong settings and log files.
            out = std::filesystem::path(path);
        }
        CoTaskMemFree(path);
        return out;
    }

    std::filesystem::path createTmpDir()
    {
        // The narrow GetTempPath() would give the temporary directory in the
        // ANSI code page, which u8path() then read as UTF-8; a user whose
        // account name is not ASCII got a name that was neither.
        std::filesystem::path out;
        WCHAR path[MAX_PATH];
        const DWORD r = GetTempPathW(MAX_PATH, path);
        if (r)
        {
            GUID guid;
            CoCreateGuid(&guid);
            const uint8_t* guidP = reinterpret_cast<const uint8_t*>(&guid);
            std::wstring name;
            for (int i = 0; i < 16; ++i)
            {
                WCHAR buf[3] = L"";
                swprintf_s(buf, 3, L"%02x", guidP[i]);
                name += buf;
            }

            // The returned length excludes the terminator, and the path
            // already ends with a separator.
            out = std::filesystem::path(std::wstring(path, r) + name);
            CreateDirectoryW(out.c_str(), NULL);
        }
        return out;
    }
}
