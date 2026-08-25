# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

# Compile a directory of SVG icons into generated sources:
#
#     ftk_icon_resources(<variable> <directory> <namespace>)
#
# The directory's SVG files are globbed -- adding an icon is adding a
# file, and CONFIGURE_DEPENDS re-runs CMake when one appears -- each is
# run through ftk-resource, and <namespace>/IconResources.h is
# generated declaring getIconResources() in <namespace>: every icon
# keyed by its file name, for registering with the icon system in one
# loop. The generated sources are appended to <variable> for the
# caller's add_library().
#
# The caller adds ${CMAKE_CURRENT_BINARY_DIR} to the target's include
# directories and add_dependencies() on ftk-resource. The generated
# files sit in a subdirectory named for the namespace so that a header
# named for a resource -- Time.svg -- cannot shadow a system header --
# <time.h> -- on a case insensitive file system.
function(ftk_icon_resources VARIABLE DIRECTORY NAMESPACE)
    file(GLOB ICON_RESOURCES CONFIGURE_DEPENDS ${DIRECTORY}/*.svg)
    set(ICON_EXTERNS)
    set(ICON_ENTRIES)
    set(ICON_SOURCE)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE})
    foreach(ICON ${ICON_RESOURCES})
        get_filename_component(ICON_BASE ${ICON} NAME_WE)
        string(MAKE_C_IDENTIFIER ${ICON_BASE} ICON_VAR)
        string(APPEND ICON_EXTERNS "    extern std::vector<uint8_t> ${ICON_VAR};\n")
        string(APPEND ICON_ENTRIES "            { \"${ICON_BASE}\", &${ICON_VAR} },\n")
        add_custom_command(
            OUTPUT
                ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}/${ICON_BASE}.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}/${ICON_BASE}.h
            COMMAND ftk-resource ${ICON} ${NAMESPACE}/${ICON_BASE} ${NAMESPACE}
            DEPENDS ${ICON})
        list(APPEND ICON_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}/${ICON_BASE}.cpp)
    endforeach()
    set(ICON_NAMESPACE ${NAMESPACE})
    configure_file(
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ftkIconResources.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}/IconResources.h
        @ONLY)
    configure_file(
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ftkIconResources.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}/IconResources.cpp
        @ONLY)
    list(APPEND ICON_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}/IconResources.cpp)
    list(APPEND ${VARIABLE} ${ICON_SOURCE})
    set(${VARIABLE} ${${VARIABLE}} PARENT_SCOPE)
endfunction()
