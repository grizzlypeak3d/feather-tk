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
    enum class FTK_CORE_API_TYPE SplitOptions
    {
        DiscardEmpty,
        KeepEmpty
    };

    //! Split a string.
    FTK_CORE_API std::vector<std::string> split(
        const std::string&,
        char delimiter,
        SplitOptions = SplitOptions::DiscardEmpty);

    //! Split a string.
    FTK_CORE_API std::vector<std::string> split(
        const std::string&,
        const std::vector<char>& delimiters,
        SplitOptions = SplitOptions::DiscardEmpty);

    //! Split a string on new-lines.
    FTK_CORE_API std::vector<std::string> splitLines(const std::string&);

    //! Join a list of strings.
    FTK_CORE_API std::string join(const std::vector<std::string>&, char delimiter);

    //! Join a list of strings.
    FTK_CORE_API std::string join(const std::vector<std::string>&, const std::string& delimiter);

    //! Join a list of strings with quotes.
    FTK_CORE_API std::string quotes(const std::vector<std::string>&);

    //! Convert to upper case.
    FTK_CORE_API std::string toUpper(const std::string&);

    //! Convert to lower case.
    FTK_CORE_API std::string toLower(const std::string&);

    //! Convert to sentence case (first word capitalized).
    FTK_CORE_API std::string toSentenceCase(const std::string&);

    //! Remove trailing newlines.
    FTK_CORE_API void removeTrailingNewlines(std::string&);

    //! Which end of an elided string to keep.
    enum class FTK_CORE_API_TYPE ElideMode
    {
        //! Keep the beginning: "ViewLayer.Combi..."
        Right,

        //! Keep the end, for names that differ where they finish, such as a
        //! path or a layer: "...yer.Combined"
        Left,

        //! Keep both ends: "ViewLa...bined"
        Middle
    };

    //! Elide a string to at most the given number of characters, not counting
    //! the ellipsis.
    FTK_CORE_API std::string elide(
        const std::string&,
        size_t = 32,
        ElideMode = ElideMode::Right);

    //! Case compare.
    enum class FTK_CORE_API_TYPE CaseCompare
    {
        Sensitive,
        Insensitive
    };

    //! Compare strings.
    FTK_CORE_API bool compare(
        const std::string&,
        const std::string&,
        CaseCompare = CaseCompare::Sensitive);

    //! Does a string contain the sub string?
    FTK_CORE_API bool contains(
        const std::string&,
        const std::string& substr,
        CaseCompare = CaseCompare::Sensitive);

    //! Does a string match the wildcard pattern? "*" matches any run of
    //! characters including an empty one, and "?" matches any single
    //! character; the pattern has to cover the whole string.
    FTK_CORE_API bool matchWildcard(
        const std::string&,
        const std::string& pattern,
        CaseCompare = CaseCompare::Sensitive);

    //! Convert a regular string to a wide string.
    FTK_CORE_API std::wstring toWide(const std::string&);

    //! Convert a wide string to a regular string.
    FTK_CORE_API std::string fromWide(const std::wstring&);

    //! Get a boolean label.
    FTK_CORE_API std::string boolLabel(bool);

    //! Convert a boolean label.
    FTK_CORE_API bool toBool(const std::string&);

    //! Get filler text.
    FTK_CORE_API std::vector<std::string> getLoremIpsum();

    //! Get filler text.
    FTK_CORE_API std::string getLoremIpsum(size_t lineCount);

    ///@}

    //! \name UTF-8
    //! Steps land on code point boundaries, so a cursor that moves by
    //! them cannot split a character.
    ///@{

    //! Get the next code point boundary after the byte index.
    FTK_CORE_API size_t utf8Next(const std::string&, size_t byteIndex);

    //! Get the previous code point boundary before the byte index.
    FTK_CORE_API size_t utf8Prev(const std::string&, size_t byteIndex);

    //! Get the number of code points in the first byteCount bytes.
    FTK_CORE_API size_t utf8Count(
        const std::string&,
        size_t byteCount = std::string::npos);

    //! Snap a byte index down to a code point boundary.
    FTK_CORE_API size_t utf8Align(const std::string&, size_t byteIndex);

    ///@}
}
