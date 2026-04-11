// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Memory.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        Endian,
        "MSB",
        "LSB");

    void swapEndian(
        void* in,
        size_t wordCount,
        size_t wordSize)
    {
        FTK_ASSERT(in);
        FTK_ASSERT(2 == wordSize || 4 == wordSize || 8 == wordSize);

        uint8_t* p = reinterpret_cast<uint8_t*>(in);
        switch (wordSize)
        {
        case 2:
            while (wordCount--)
            {
                std::swap(p[0], p[1]);
                p += 2;
            }
            break;
        case 4:
            while (wordCount--)
            {
#if defined(__GNUC__) || defined(__clang__)
                uint32_t val;
                __builtin_memcpy(&val, p, 4);
                val = __builtin_bswap32(val);
                __builtin_memcpy(p, &val, 4);
#else
                std::swap(p[0], p[3]);
                std::swap(p[1], p[2]);
#endif
                p += 4;
            }
            break;
        case 8:
            while (wordCount--)
            {
#if defined(__GNUC__) || defined(__clang__)
                uint64_t val;
                __builtin_memcpy(&val, p, 8);
                val = __builtin_bswap64(val);
                __builtin_memcpy(p, &val, 8);
#else
                std::swap(p[0], p[7]);
                std::swap(p[1], p[6]);
                std::swap(p[2], p[5]);
                std::swap(p[3], p[4]);
#endif
                p += 8;
            }
            break;
        default:
            FTK_ASSERT(false);
            break;
        }
    }

    void swapEndian(
        const void* in,
        void*       out,
        size_t      wordCount,
        size_t      wordSize)
    {
        FTK_ASSERT(in);
        FTK_ASSERT(out);
        FTK_ASSERT(in != out);
        FTK_ASSERT(2 == wordSize || 4 == wordSize || 8 == wordSize);

        const uint8_t* inP = reinterpret_cast<const uint8_t*>(in);
        uint8_t* outP = reinterpret_cast<uint8_t*>(out);
        switch (wordSize)
        {
        case 2:
            while (wordCount--)
            {
                outP[0] = inP[1];
                outP[1] = inP[0];
                inP += 2;
                outP += 2;
            }
            break;
        case 4:
            while (wordCount--)
            {
#if defined(__GNUC__) || defined(__clang__)
                uint32_t val;
                __builtin_memcpy(&val, inP, 4);
                val = __builtin_bswap32(val);
                __builtin_memcpy(outP, &val, 4);
#else
                outP[0] = inP[3];
                outP[1] = inP[2];
                outP[2] = inP[1];
                outP[3] = inP[0];
#endif
                inP += 4;
                outP += 4;
            }
            break;
        case 8:
            while (wordCount--)
            {
#if defined(__GNUC__) || defined(__clang__)
                uint64_t val;
                __builtin_memcpy(&val, inP, 8);
                val = __builtin_bswap64(val);
                __builtin_memcpy(outP, &val, 8);
#else
                outP[0] = inP[7];
                outP[1] = inP[6];
                outP[2] = inP[5];
                outP[3] = inP[4];
                outP[4] = inP[3];
                outP[5] = inP[2];
                outP[6] = inP[1];
                outP[7] = inP[0];
#endif
                inP += 8;
                outP += 8;
            }
            break;
        default:
            FTK_ASSERT(false);
            break;
        }
    }

    std::string toBitString(uint8_t value)
    {
        std::string out;
        out.reserve(8);
        for (int i = 7; i >= 0; --i)
        {
            out.push_back((value & (0x80 >> i)) ? '1' : '0');
        }
        return out;
    }

    std::string toBitString(uint16_t value)
    {
        std::string out;
        out.reserve(16);
        for (int i = 15; i >= 0; --i)
        {
            out.push_back((value & (0x8000 >> i)) ? '1' : '0');
        }
        return out;
    }
}
