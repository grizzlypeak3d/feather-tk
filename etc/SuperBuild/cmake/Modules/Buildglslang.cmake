include(ExternalProject)

# The GLSL compiler, for the GPU renderer where it draws with Vulkan: the
# shaders are GLSL, Vulkan takes SPIR-V, and the ones OpenColorIO writes are
# not known until the program runs. Without the optimizer, which wants
# SPIRV-Tools and is not needed to compile.
set(glslang_GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git")
set(glslang_GIT_TAG "16.6.0")

set(glslang_ARGS
    ${ftk_DEPS_ARGS}
    -DENABLE_OPT=OFF
    -DENABLE_HLSL=OFF
    -DENABLE_GLSLANG_BINARIES=OFF
    -DGLSLANG_TESTS=OFF
    -DBUILD_EXTERNAL=OFF)

ExternalProject_Add(
    glslang
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glslang
    GIT_REPOSITORY ${glslang_GIT_REPOSITORY}
    GIT_TAG ${glslang_GIT_TAG}
    LIST_SEPARATOR |
    CMAKE_ARGS ${glslang_ARGS})
