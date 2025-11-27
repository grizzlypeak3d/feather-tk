include(ExternalProject)

set(pybind11_GIT_REPOSITORY "https://github.com/pybind/pybind11.git")
# \bug This needs to match the version used by OpenTimelineIO
set(pybind11_GIT_TAG "v2.13.6")

set(pybind11_ARGS
    ${ftk_DEPS_ARGS}
    -DPYBIND11_TEST=OFF)

ExternalProject_Add(
    pybind11
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pybind11
    GIT_REPOSITORY ${pybind11_GIT_REPOSITORY}
    GIT_TAG ${pybind11_GIT_TAG}
    LIST_SEPARATOR |
    CMAKE_ARGS ${pybind11_ARGS})

