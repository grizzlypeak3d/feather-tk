// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#define FTK_ENUM_BIND(M, ENUM) \
    M.def("get" #ENUM "Enums", &get##ENUM##Enums); \
    M.def("getLabel", [](ENUM v) { return getLabel(v); }); \
    M.def("to_string", [](ENUM v) { return to_string(v); }); \
    M.def("from_string", [](const std::string& s, ENUM& v) { return from_string(s, v); })

    // \bug ImportError: Internal error while parsing type signature (1)
    //M.def("get" #ENUM "Labels", &get##ENUM##Labels); \

