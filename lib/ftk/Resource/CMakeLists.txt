set(HEADERS)
set(SOURCE)

# The fonts and images are named, since code refers to them one by one.
set(RESOURCES
    etc/Fonts/NotoMono-Regular.ttf
    etc/Fonts/NotoSans-Bold.ttf
    etc/Fonts/NotoSans-Regular.ttf
    etc/Fonts/NotoSansSymbols2-Regular.ttf
    etc/Fonts/NotoSansCJK-Regular.otf
    etc/Images/Charlie.png
    etc/Images/TestDrawing.svg)
list(TRANSFORM RESOURCES PREPEND ${PROJECT_SOURCE_DIR}/)

# The icons are globbed and registered by a generated map, so adding an
# icon is adding a file to etc/Icons; the icon system picks up whatever
# is there. CONFIGURE_DEPENDS makes a new file re-run CMake.
file(GLOB ICON_RESOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/etc/Icons/*.svg)
set(ICON_EXTERNS)
set(ICON_ENTRIES)
foreach(ICON ${ICON_RESOURCES})
    get_filename_component(ICON_BASE ${ICON} NAME_WE)
    string(MAKE_C_IDENTIFIER ${ICON_BASE} ICON_VAR)
    string(APPEND ICON_EXTERNS "    extern std::vector<uint8_t> ${ICON_VAR};\n")
    string(APPEND ICON_ENTRIES "            { \"${ICON_BASE}\", &${ICON_VAR} },\n")
endforeach()
# The generated files live in a subdirectory and are included as
# <ftkResource/Name.h>: a header named for a resource -- Time.svg --
# in the include path root would shadow a system header -- <time.h> --
# on a case insensitive file system.
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ftkResource)
configure_file(IconResources.h.in ${CMAKE_CURRENT_BINARY_DIR}/ftkResource/IconResources.h @ONLY)
configure_file(IconResources.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/ftkResource/IconResources.cpp @ONLY)
list(APPEND SOURCE ${CMAKE_CURRENT_BINARY_DIR}/ftkResource/IconResources.cpp)

list(APPEND RESOURCES ${ICON_RESOURCES})
foreach(RESOURCE ${RESOURCES})
    get_filename_component(RESOURCE_BASE ${RESOURCE} NAME_WE)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/ftkResource/${RESOURCE_BASE}.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/ftkResource/${RESOURCE_BASE}.h
        COMMAND ftk-resource ${RESOURCE} ftkResource/${RESOURCE_BASE} ftk_resource
        DEPENDS ${RESOURCE})
    list(APPEND SOURCE ${CMAKE_CURRENT_BINARY_DIR}/ftkResource/${RESOURCE_BASE}.cpp)
endforeach()

# Static, whatever the rest of the build is. The resources are data at
# namespace scope with no export macro on them, so a shared build on Windows
# produces a library that exports nothing and no import library beside it:
#
#     LINK : fatal error LNK1104: cannot open file 'ftkResource.lib'
#
# CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS does not answer this the way it does for
# the dependencies: it writes a module definition file from the functions the
# objects hold, and these hold none. Marking the data instead would mean the
# generator and every extern declaration that names it.
#
# Static suits them anyway. They are an implementation detail rather than API,
# and each resource is its own object file, so a consumer links only the ones
# it names -- FontSystem and IconSystem take the ones they use and nothing more.
add_library(ftkResource STATIC ${HEADERS} ${SOURCE})

# The generated headers -- the per resource declarations and the icon
# map -- are found here; the build interface only, since the resources
# are an implementation detail rather than installed API.
target_include_directories(ftkResource PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
set_target_properties(ftkResource PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(ftkResource PUBLIC FTK_CJK)

target_link_libraries(ftkResource ZLIB::ZLIB)

set_target_properties(ftkResource PROPERTIES FOLDER lib)
set_target_properties(ftkResource PROPERTIES PUBLIC_HEADER "${HEADERS}")

add_dependencies(ftkResource ftk-resource)

install(
    TARGETS ftkResource
    EXPORT ftkResourceTargets
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/Resource COMPONENT dev)
install(
    EXPORT ftkResourceTargets
    FILE ftkResourceTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::
    COMPONENT dev)
