include(ExternalProject)

set(Freetype_GIT_REPOSITORY "https://github.com/freetype/freetype.git")
set(Freetype_GIT_TAG "VER-2-14-1")

set(Freetype_DEPS)
if(ftk_ZLIB)
    list(APPEND Freetype_DEPS ZLIB)
endif()

set(Freetype_ARGS
    ${ftk_DEPS_ARGS}
    -DCMAKE_INSTALL_LIBDIR=lib
    -DFT_REQUIRE_ZLIB=ON
    -DFT_DISABLE_BZIP2=ON
    -DFT_DISABLE_PNG=ON
    -DFT_DISABLE_HARFBUZZ=ON
    -DFT_DISABLE_BROTLI=ON)

ExternalProject_Add(
    Freetype
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/Freetype
    DEPENDS ${Freetype_DEPS}
    GIT_REPOSITORY ${Freetype_GIT_REPOSITORY}
    GIT_TAG ${Freetype_GIT_TAG}
    LIST_SEPARATOR |
    CMAKE_ARGS ${Freetype_ARGS})
