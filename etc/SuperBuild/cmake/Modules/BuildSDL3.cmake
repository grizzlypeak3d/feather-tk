include(ExternalProject)
find_package(Git REQUIRED)

set(SDL3_GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git")
set(SDL3_GIT_TAG "release-3.4.16")

# One or the other. "NOT" is not an operator outside if(): written into
# set() it made the list "NOT;ON", which is true, and a shared build
# installed the static library as well.
set(SDL3_STATIC ON)
if(BUILD_SHARED_LIBS)
    set(SDL3_STATIC OFF)
endif()
set(SDL3_ARGS
    ${ftk_DEPS_ARGS}
    -DSDL_SHARED=${BUILD_SHARED_LIBS}
    -DSDL_STATIC=${SDL3_STATIC}
    -DSDL_DUMMYVIDEO=OFF
    -DSDL_OPENGL=ON
    -DSDL_OPENGLES=OFF
    -DSDL_ALSA=ON
    -DSDL_OSS=OFF
    -DSDL_JACK=OFF
    -DSDL_PIPEWIRE=ON
    -DSDL_PULSEAUDIO=ON
    -DSDL_SNDIO=OFF
    -DSDL_X11=ON
    -DSDL_X11_XSCRNSAVER=OFF
    -DSDL_X11_XTEST=OFF
    -DSDL_WAYLAND=ON
    -DSDL_RPI=OFF
    -DSDL_DIRECTX=OFF
    -DSDL_RENDER_D3D=OFF
    -DSDL_RENDER_METAL=OFF
    -DSDL_VIVANTE=OFF
    # Vulkan and Metal are for SDL's GPU API, which the GPU renderer draws
    # with: Metal on macOS and Vulkan elsewhere. SDL loads the Vulkan
    # library when a device is asked for, so nothing new is linked.
    -DSDL_VULKAN=ON
    -DSDL_METAL=ON
    -DSDL_GPU=ON
    -DSDL_KMSDRM=OFF
    -DSDL_RENDER=OFF
    -DSDL_JOYSTICK=OFF
    -DSDL_HAPTIC=OFF
    -DSDL_POWER=OFF
    -DSDL_SENSOR=OFF
    -DSDL_DIALOG=OFF
    # SDL builds its test programs by default when it is the main project,
    # which it is in this superbuild: testffmpeg finds whatever FFmpeg is in
    # the install prefix and fails against it.
    -DSDL_TEST_LIBRARY=OFF
    -DSDL_TESTS=OFF
    -DSDL_EXAMPLES=OFF)

# Two patches. One from upstream pull request #14157 (open): the Metal GPU
# driver makes a texture over an existing CVPixelBuffer's surface, which
# is how a frame the hardware decoded is drawn without a copy. Drop it
# when a release carries the change. The other is ours: on iOS a number
# input gets the keyboard with the punctuation rather than the decimal
# pad, which has no minus sign and no colon.
ExternalProject_Add(
    SDL3
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/SDL3
    GIT_REPOSITORY ${SDL3_GIT_REPOSITORY}
    GIT_TAG ${SDL3_GIT_TAG}
    PATCH_COMMAND ${CMAKE_COMMAND}
        -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
        -DPATCH_SOURCE_DIR=${CMAKE_CURRENT_BINARY_DIR}/SDL3/src/SDL3
        -DPATCH_FILE=${CMAKE_CURRENT_SOURCE_DIR}/SDL3-patch/pixelbuffer.patch
        -P ${CMAKE_CURRENT_LIST_DIR}/ApplyPatch.cmake
    COMMAND ${CMAKE_COMMAND}
        -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
        -DPATCH_SOURCE_DIR=${CMAKE_CURRENT_BINARY_DIR}/SDL3/src/SDL3
        -DPATCH_FILE=${CMAKE_CURRENT_SOURCE_DIR}/SDL3-patch/keyboard.patch
        -P ${CMAKE_CURRENT_LIST_DIR}/ApplyPatch.cmake
    LIST_SEPARATOR |
    CMAKE_ARGS ${SDL3_ARGS})
