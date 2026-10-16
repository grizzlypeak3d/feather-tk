# Cross-compiling for iOS, for the super builds and the libraries alike:
# CMake has the platform built in, and this file only names it, since the
# super builds hand a toolchain file on to the dependencies and not the
# system name. Use with the Ninja or Xcode generator:
#
#     cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=path/to/etc/iOS/toolchain.cmake \
#         -DIOS_SDK=iphonesimulator ...
#
# IOS_SDK is iphonesimulator (the default) or iphoneos; the architecture is
# arm64 for both -- Apple silicon runs the simulator natively.
set(CMAKE_SYSTEM_NAME iOS)
if(NOT IOS_SDK)
    set(IOS_SDK iphonesimulator)
endif()
set(CMAKE_OSX_SYSROOT ${IOS_SDK} CACHE STRING "")
set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "")
# Named as well: CMake leaves it empty for iOS, and a dependency that
# lowercases it to pick its assembly (libjpeg-turbo) fails on the empty
# string.
set(CMAKE_SYSTEM_PROCESSOR arm64)
if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 17.0 CACHE STRING "")
endif()
# Swift is told the target as well; with the Ninja generator CMake passes
# it the SDK and nothing else.
if(IOS_SDK STREQUAL "iphonesimulator")
    set(CMAKE_Swift_COMPILER_TARGET arm64-apple-ios${CMAKE_OSX_DEPLOYMENT_TARGET}-simulator)
else()
    set(CMAKE_Swift_COMPILER_TARGET arm64-apple-ios${CMAKE_OSX_DEPLOYMENT_TARGET})
endif()
# Programs cannot run during the build; everything is found in the install
# prefix the super build is given, not on the host.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO)
set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
