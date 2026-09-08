include(ExternalProject)

set(nanobind_GIT_REPOSITORY "https://github.com/wjakob/nanobind.git")
set(nanobind_GIT_TAG "v3.0.1")

set(nanobind_ARGS
    ${ftk_DEPS_ARGS}
    -DNB_TEST=OFF
    -DNB_USE_SUBMODULE_DEPS=ON)

ExternalProject_Add(
    nanobind
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/nanobind
    GIT_REPOSITORY ${nanobind_GIT_REPOSITORY}
    GIT_TAG ${nanobind_GIT_TAG}
    LIST_SEPARATOR |
    CMAKE_ARGS ${nanobind_ARGS})
