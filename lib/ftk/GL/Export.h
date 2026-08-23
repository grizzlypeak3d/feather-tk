// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

// The platform layer -- FEATHER_TK_EXPORT and the rest -- is shared, and lives
// with the core library.
#include <ftk/Core/Export.h>

// The macros for the OpenGL library. Each library in this project has its own set: one
// shared between them would be defined for whichever library is being built,
// so a library compiling a sibling's headers would read the sibling's API as
// dllexport where it wants dllimport. Functions survive that -- the linker
// takes them from the import library -- and data does not.
#if defined(FTK_GL_STATIC)
#    define FTK_GL_API
#    define FTK_GL_API_TYPE
#    define FTK_GL_API_TEMPLATE_CLASS(...)
#    define FTK_GL_API_TEMPLATE_STRUCT(...)
#    define FTK_GL_LOCAL
#else
#    if defined(FTK_GL_EXPORTS)
#        define FTK_GL_API FEATHER_TK_EXPORT
#        define FTK_GL_API_TYPE FEATHER_TK_EXPORT_TYPE
#        define FTK_GL_API_TEMPLATE_CLASS(...)                                             FEATHER_TK_EXPORT_TEMPLATE(class, __VA_ARGS__)
#        define FTK_GL_API_TEMPLATE_STRUCT(...)                                            FEATHER_TK_EXPORT_TEMPLATE(struct, __VA_ARGS__)
#    else
#        define FTK_GL_API FEATHER_TK_IMPORT
#        define FTK_GL_API_TYPE FEATHER_TK_IMPORT_TYPE
#        define FTK_GL_API_TEMPLATE_CLASS(...)                                             FEATHER_TK_IMPORT_TEMPLATE(class, __VA_ARGS__)
#        define FTK_GL_API_TEMPLATE_STRUCT(...)                                            FEATHER_TK_IMPORT_TEMPLATE(struct, __VA_ARGS__)
#    endif
#    define FTK_GL_LOCAL FEATHER_TK_HIDDEN
#endif
