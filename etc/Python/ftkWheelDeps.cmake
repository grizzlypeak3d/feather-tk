# The dependencies of a wheel build. pip runs one CMake project and nothing
# before it, so the super build is run from here, at configure time, into the
# binary directory; everything it installs goes into the wheel with
# feather-tk, so that all the module loads is inside the package.
#
# A prefix that is already complete is used as it is: keep scikit-build-core's
# build directory (build-dir in pyproject.toml) to build them once, or point
# ftk_WHEEL_DEPS at a prefix built some other way.

set(ftk_WHEEL_DEPS "${CMAKE_BINARY_DIR}/deps" CACHE PATH "Dependencies prefix for wheel builds")
set(ftk_WHEEL_DEPS_STAMP "${ftk_WHEEL_DEPS}/.ftk-wheel-deps")

if(NOT EXISTS "${ftk_WHEEL_DEPS_STAMP}")
    set(config ${CMAKE_BUILD_TYPE})
    if(NOT config)
        set(config Release)
    endif()
    # The super build builds shared libraries without Python: nanobind comes
    # from pip, as a build requirement.
    set(args
        -S ${PROJECT_SOURCE_DIR}/etc/SuperBuild
        -B ${CMAKE_BINARY_DIR}/deps-build
        -G ${CMAKE_GENERATOR}
        -DCMAKE_BUILD_TYPE=${config}
        -DCMAKE_INSTALL_PREFIX=${ftk_WHEEL_DEPS}
        -DCMAKE_PREFIX_PATH=${ftk_WHEEL_DEPS}
        -DBUILD_SHARED_LIBS=ON
        -Dftk_PYTHON=OFF)
    if(CMAKE_GENERATOR_PLATFORM)
        list(APPEND args -A ${CMAKE_GENERATOR_PLATFORM})
    endif()
    if(CMAKE_GENERATOR_TOOLSET)
        list(APPEND args -T ${CMAKE_GENERATOR_TOOLSET})
    endif()
    # What scikit-build-core decided for this build -- the compilers, the
    # Ninja it installed, and on macOS the architecture and deployment
    # target of the wheel -- has to hold for the dependencies as well.
    foreach(var
        CMAKE_C_COMPILER
        CMAKE_CXX_COMPILER
        CMAKE_MAKE_PROGRAM
        CMAKE_OSX_ARCHITECTURES
        CMAKE_OSX_DEPLOYMENT_TARGET
        ftk_SDL2
        ftk_SDL3
        ftk_nfd)
        if(DEFINED ${var} AND NOT "${${var}}" STREQUAL "")
            list(APPEND args "-D${var}=${${var}}")
        endif()
    endforeach()

    message(STATUS "Building the wheel dependencies into ${ftk_WHEEL_DEPS}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${args}
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/deps-build --config ${config}
        COMMAND_ERROR_IS_FATAL ANY)
    file(TOUCH "${ftk_WHEEL_DEPS_STAMP}")
endif()

list(PREPEND CMAKE_PREFIX_PATH ${ftk_WHEEL_DEPS})

# Everything but what only means something on this machine: pkg-config
# files hold its absolute paths.
install(
    DIRECTORY ${ftk_WHEEL_DEPS}/
    DESTINATION .
    USE_SOURCE_PERMISSIONS
    PATTERN .ftk-wheel-deps EXCLUDE
    PATTERN pkgconfig EXCLUDE
    PATTERN "glad_*" EXCLUDE)
