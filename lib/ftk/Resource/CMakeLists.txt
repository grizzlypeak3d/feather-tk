set(HEADERS)
set(SOURCE)

# The fonts and images are named, since code refers to them one by one.
set(RESOURCES
    etc/Fonts/NotoMono-Regular.ttf
    etc/Fonts/NotoSans-Bold.ttf
    etc/Fonts/NotoSans-Regular.ttf
    etc/Fonts/NotoSansSymbols2-Regular.ttf
    etc/Images/Charlie.png
    etc/Images/TestDrawing.svg)
if(ftk_CJK)
    list(APPEND RESOURCES etc/Fonts/NotoSansCJK-Regular.otf)
endif()
list(TRANSFORM RESOURCES PREPEND ${PROJECT_SOURCE_DIR}/)

# The icons go through ftk_icon_resources(): adding an icon is adding a
# file to etc/Icons, and the icon system registers the generated map.
ftk_icon_resources(SOURCE ${PROJECT_SOURCE_DIR}/etc/Icons ftk_resource)
foreach(RESOURCE ${RESOURCES})
    get_filename_component(RESOURCE_BASE ${RESOURCE} NAME_WE)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/ftk_resource/${RESOURCE_BASE}.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/ftk_resource/${RESOURCE_BASE}.h
        COMMAND ${ftk_RESOURCE_COMMAND} ${RESOURCE} ftk_resource/${RESOURCE_BASE} ftk_resource
        DEPENDS ${RESOURCE})
    list(APPEND SOURCE ${CMAKE_CURRENT_BINARY_DIR}/ftk_resource/${RESOURCE_BASE}.cpp)
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

if(ftk_CJK)
    target_compile_definitions(ftkResource PUBLIC FTK_CJK)
endif()

target_link_libraries(ftkResource ZLIB::ZLIB)

set_target_properties(ftkResource PROPERTIES FOLDER lib)
set_target_properties(ftkResource PROPERTIES PUBLIC_HEADER "${HEADERS}")

if(NOT ftk_RESOURCE)
    add_dependencies(ftkResource ftk-resource)
endif()

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
