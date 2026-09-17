# Python wheels: the feather_tk package with the libraries it loads, and the
# headers and CMake package to build against them.
#
# Read by the main project when scikit-build-core runs it (see
# pyproject.toml), before the options, the way "cmake -C" would be. There is
# no local.cmake here: a wheel should build the same on every machine.

set(ftk_PYTHON ON CACHE BOOL "")
set(ftk_PYTHON_STABLE_ABI ON CACHE BOOL "")
# The package is the wheel's install directory (wheel.install-dir), so the
# module goes at its top and the libraries in lib beside it.
set(ftk_PYTHON_INSTALL_DIR "." CACHE STRING "")
set(ftk_TESTS OFF CACHE BOOL "")
set(ftk_EXAMPLES OFF CACHE BOOL "")

# Shared, for the reason default.cmake gives, and so the tlRender and DJV
# wheels can load the libraries in this one rather than carry their own
# copies alongside it.
set(BUILD_SHARED_LIBS ON CACHE BOOL "")

# libGL rather than the GLVND libraries FindOpenGL prefers: libGL is among the
# libraries a manylinux wheel may take from the system, and libOpenGL is not,
# so the repair tool would copy it into the wheel.
set(OpenGL_GL_PREFERENCE LEGACY CACHE STRING "")
