// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#define FTK_VERSION_MAJOR 0
#define FTK_VERSION_MINOR 11
#define FTK_VERSION_PATCH 0
#define FTK_VERSION_DEV "-dev"

#define FTK_VERSION_STR_(x) #x
#define FTK_VERSION_STR(x) FTK_VERSION_STR_(x)
#define FTK_VERSION_FULL \
    FTK_VERSION_STR(FTK_VERSION_MAJOR) "." \
    FTK_VERSION_STR(FTK_VERSION_MINOR) "." \
    FTK_VERSION_STR(FTK_VERSION_PATCH) FTK_VERSION_DEV
