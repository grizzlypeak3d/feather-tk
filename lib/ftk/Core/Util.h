// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

//! Convenience macro for making a class non-copyable.
#define FTK_NON_COPYABLE(CLASS) \
    CLASS(const CLASS&) = delete; \
    CLASS& operator = (const CLASS&) = delete

//! Convenience macro for private implementations.
//! 
//! Required includes:
//! * memory
#define FTK_PRIVATE() \
    struct Private; \
    std::unique_ptr<Private> _p

//! Convenience macro for accessing private implementations.
#define FTK_P() \
    auto& p = *_p

//! Convenience macro for enum utilities.
//! 
//! Required includes:
//! * iostream
//! * string
//! * vector
#define FTK_ENUM(ENUM) \
    std::vector<ENUM> get##ENUM##Enums(); \
    const std::vector<std::string>& get##ENUM##Labels(); \
    const std::string& getLabel(ENUM); \
    std::string to_string(ENUM); \
    bool from_string(const std::string&, ENUM&); \
    std::ostream& operator << (std::ostream&, ENUM)

//! Implementation macro for enum utilities.
//! 
//! Required includes:
//! * ftk/core/Error.h
//! * ftk/core/String.h
//! * algorithm
//! * array
//! * sstream
#define FTK_ENUM_IMPL(ENUM, ...) \
    std::vector<ENUM> get##ENUM##Enums() \
    { \
        std::vector<ENUM> out; \
        for (std::size_t i = 0; i < static_cast<std::size_t>(ENUM::Count); ++i) \
        { \
            out.push_back(static_cast<ENUM>(i)); \
        } \
        return out; \
    } \
    \
    const std::vector<std::string>& get##ENUM##Labels() \
    { \
        static std::vector<std::string> data = { __VA_ARGS__ }; \
        return data; \
    } \
    \
    const std::string& getLabel(ENUM value) \
    { \
        static const std::array<std::string, static_cast<std::size_t>(ENUM::Count)> data = { __VA_ARGS__ }; \
        return data[static_cast<std::size_t>(value)]; \
    } \
    \
    std::string to_string(ENUM value) \
    { \
        return getLabel(value); \
    } \
    \
    bool from_string(const std::string& s, ENUM& value) \
    { \
        bool out = false; \
        const auto& labels = get##ENUM##Labels(); \
        const auto i = std::find_if( \
            labels.begin(), \
            labels.end(), \
            [s](const std::string& value) \
            { \
                return ftk::compare(s, value, ftk::CaseCompare::Insensitive); \
            }); \
        if (i != labels.end()) \
        { \
            value = static_cast<ENUM>(i - labels.begin()); \
            out = true; \
        } \
        return out; \
    } \
    \
    std::ostream& operator << (std::ostream& os, ENUM in) \
    { \
        os << get##ENUM##Labels()[static_cast<std::size_t>(in)]; \
        return os; \
    }
