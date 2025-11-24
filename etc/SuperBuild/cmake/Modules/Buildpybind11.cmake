include(ExternalProject)

set(pybind11_GIT_REPOSITORY "https://github.com/pybind/pybind11.git")
set(pybind11_GIT_TAG "v3.0.1")

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
