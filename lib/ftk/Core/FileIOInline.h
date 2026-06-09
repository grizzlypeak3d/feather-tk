// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

namespace ftk
{
    inline MemFile::MemFile(const std::shared_ptr<void>& f, const uint8_t* p, size_t size) :
        f(f),
        p(p),
        size(size)
    {}

    inline bool MemFile::operator == (const MemFile& other) const
    {
        return
            f == other.f &&
            p == other.p &&
            size == other.size;
    }

    inline bool MemFile::operator != (const MemFile& other) const
    {
        return !(*this == other);
    }
}
