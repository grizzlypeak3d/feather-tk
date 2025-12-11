// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

// For an explanation of how these export defines work, see:
// https://github.com/PixarAnimationStudios/OpenUSD/blob/dev/pxr/base/arch/export.h
#if defined(_WINDOWS)
#    if defined(__GNUC__) && __GNUC__ >= 4 || defined(__clang__)
#        define FEATHER_TK_EXPORT __attribute__((dllexport))
#        define FEATHER_TK_IMPORT __attribute__((dllimport))
#        define FEATHER_TK_HIDDEN
#        define FEATHER_TK_EXPORT_TYPE
#        define FEATHER_TK_IMPORT_TYPE
#    else
#        define FEATHER_TK_EXPORT __declspec(dllexport)
#        define FEATHER_TK_IMPORT __declspec(dllimport)
#        define FEATHER_TK_HIDDEN
#        define FEATHER_TK_EXPORT_TYPE
#        define FEATHER_TK_IMPORT_TYPE
#    endif
#elif defined(__GNUC__) && __GNUC__ >= 4 || defined(__clang__)
#    define FEATHER_TK_EXPORT __attribute__((visibility("default")))
#    define FEATHER_TK_IMPORT
#    define FEATHER_TK_HIDDEN __attribute__((visibility("hidden")))
#    if defined(__clang__)
#        define FEATHER_TK_EXPORT_TYPE                                     \
            __attribute__((type_visibility("default")))
#    else
#        define FEATHER_TK_EXPORT_TYPE                                     \
            __attribute__((visibility("default")))
#    endif
#    define FEATHER_TK_IMPORT_TYPE
#else
#    define FEATHER_TK_EXPORT
#    define FEATHER_TK_IMPORT
#    define FEATHER_TK_HIDDEN
#    define FEATHER_TK_EXPORT_TYPE
#    define FEATHER_TK_IMPORT_TYPE
#endif
#define FEATHER_TK_EXPORT_TEMPLATE(type, ...)
#define FEATHER_TK_IMPORT_TEMPLATE(type, ...)                              \
    extern template type FEATHER_TK_IMPORT __VA_ARGS__

#if defined(FTK_STATIC)
#    define FTK_API
#    define FTK_API_TYPE
#    define FTK_API_TEMPLATE_CLASS(...)
#    define FTK_API_TEMPLATE_STRUCT(...)
#    define FTK_LOCAL
#else
#    if defined(FTK_EXPORTS)
#        define FTK_API FEATHER_TK_EXPORT
#        define FTK_API_TYPE FEATHER_TK_EXPORT_TYPE
#        define FTK_API_TEMPLATE_CLASS(...)                               \
            FEATHER_TK_EXPORT_TEMPLATE(class, __VA_ARGS__)
#        define FTK_API_TEMPLATE_STRUCT(...)                              \
            FEATHER_TK_EXPORT_TEMPLATE(struct, __VA_ARGS__)
#    else
#        define FTK_API FEATHER_TK_IMPORT
#        define FTK_API_TYPE FEATHER_TK_IMPORT_TYPE
#        define FTK_API_TEMPLATE_CLASS(...)                               \
            FEATHER_TK_IMPORT_TEMPLATE(class, __VA_ARGS__)
#        define FTK_API_TEMPLATE_STRUCT(...)                              \
            FEATHER_TK_IMPORT_TEMPLATE(struct, __VA_ARGS__)
#    endif
#    define FTK_LOCAL FEATHER_TK_HIDDEN
#endif
