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

#include <codecvt>
#include <locale>

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
                try
                {
                    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
                    for (WCHAR* p = buf.data(), *end = buf.data() + result; p < end && *p; ++p)
                    {
                        WCHAR* p2 = p;
                        for (; p2 < end && *p2; ++p2)
                            ;
                        out.push_back(utf16.to_bytes(std::wstring(p, p2 - p)));
                        p = p2;
                    }
                }
                catch (const std::exception&)
                {
                    //! \bug How should we handle this error?
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
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
            out = utf16.to_bytes(path);
        }
        CoTaskMemFree(path);
        return out;
    }

    std::filesystem::path createTmpDir()
    {
        std::string out;
        char path[MAX_PATH];
        DWORD r = GetTempPath(MAX_PATH, path);
        if (r)
        {
            out = std::string(path);
            GUID guid;
            CoCreateGuid(&guid);
            const uint8_t* guidP = reinterpret_cast<const uint8_t*>(&guid);
            for (int i = 0; i < 16; ++i)
            {
                char buf[3] = "";
                sprintf_s(buf, 3, "%02x", guidP[i]);
                out += buf;
            }

            CreateDirectory(out.c_str(), NULL);
        }
        return std::filesystem::u8path(out);
    }
}
