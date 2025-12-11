// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <string>
#include <vector>

namespace ftk
{
    //! \name Strings
    ///@{
        
    //! Arbitrary C string size.
    constexpr size_t cStringSize = 4096;

    //! String split options.
    enum class FTK_API_TYPE SplitOptions
    {
        DiscardEmpty,
        KeepEmpty
    };

    //! Split a string.
    FTK_API std::vector<std::string> split(
        const std::string&,
        char delimeter,
        SplitOptions = SplitOptions::DiscardEmpty);

    //! Split a string.
    FTK_API std::vector<std::string> split(
        const std::string&,
        const std::vector<char>& delimeters,
        SplitOptions = SplitOptions::DiscardEmpty);

    //! Split a string on new-lines.
    FTK_API std::vector<std::string> splitLines(const std::string&);

    //! Join a list of strings.
    FTK_API std::string join(const std::vector<std::string>&, char delimeter);

    //! Join a list of strings.
    FTK_API std::string join(const std::vector<std::string>&, const std::string& delimeter);

    //! Join a list of strings with quotes.
    FTK_API std::string quotes(const std::vector<std::string>&);

    //! Convert to upper case.
    FTK_API std::string toUpper(const std::string&);

    //! Convert to lower case.
    FTK_API std::string toLower(const std::string&);

    //! Remove trailing newlines.
    FTK_API void removeTrailingNewlines(std::string&);

    //! Elide a string.
    FTK_API std::string elide(const std::string&, size_t = 32);

    //! Case compare.
    enum class FTK_API_TYPE CaseCompare
    {
        Sensitive,
        Insensitive
    };

    //! Compare strings.
    FTK_API bool compare(
        const std::string&,
        const std::string&,
        CaseCompare = CaseCompare::Sensitive);

    //! Does a string contain the sub string?
    FTK_API bool contains(
        const std::string&,
        const std::string& substr,
        CaseCompare = CaseCompare::Sensitive);

    //! Convert a regular string to a wide string.
    FTK_API std::wstring toWide(const std::string&);

    //! Convert a wide string to a regular string.
    FTK_API std::string fromWide(const std::wstring&);

    //! Get filler text.
    FTK_API std::vector<std::string> getLoremIpsum();

    //! Get filler text.
    FTK_API std::string getLoremIpsum(size_t lineCount);

    ///@}
}
