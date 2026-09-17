include(ExternalProject)

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
    -DSDL_VULKAN=OFF
    -DSDL_METAL=OFF
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

ExternalProject_Add(
    SDL3
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/SDL3
    GIT_REPOSITORY ${SDL3_GIT_REPOSITORY}
    GIT_TAG ${SDL3_GIT_TAG}
    LIST_SEPARATOR |
    CMAKE_ARGS ${SDL3_ARGS})
