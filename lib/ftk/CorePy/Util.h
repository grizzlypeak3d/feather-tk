// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#define FTK_ENUM_BIND(ENUM) \
    m.def("get" #ENUM "Enums", &get##ENUM##Enums); \
    m.def("get" #ENUM "Labels", &get##ENUM##Labels); \
    m.def("getLabel", [](ENUM v) { return getLabel(v); }); \
    m.def("to_string", [](ENUM v) { return to_string(v); }); \
    m.def("from_string", [](const std::string& s, ENUM& v) { return from_string(s, v); })

