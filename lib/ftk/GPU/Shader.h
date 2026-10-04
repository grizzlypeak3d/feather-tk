// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>

#include <cstdint>
#include <string>
#include <vector>

struct SDL_GPUDevice;
struct SDL_GPUShader;

namespace ftk
{
    namespace gpu
    {
        //! A shader's source, in each of the languages a driver takes.
        //!
        //! Metal compiles its own language from source. Vulkan takes
        //! SPIR-V, which GLSL is compiled to here, with glslang. The two
        //! say the same thing and are kept side by side.
        //!
        //! Where things go, which is SDL's layout and not a choice:
        //! - Metal: the uniforms are the stage's buffer zero, and textures
        //!   and samplers are numbered together from zero. The entry
        //!   points are "vertexMain" and "fragmentMain".
        //! - GLSL: a vertex stage's uniforms are set 1, binding 0. A
        //!   fragment stage's samplers are set 2, numbered from zero, and
        //!   its uniforms set 3, binding 0. Uniform blocks are std140, which
        //!   lays out the blocks used here the way Metal does.
        struct FTK_GPU_API_TYPE ShaderSource
        {
            std::string msl;
            std::string glsl;
        };

        //! Shader stages.
        enum class FTK_GPU_API_TYPE ShaderStage
        {
            Vertex,
            Fragment
        };

        //! Get whether this build can compile GLSL, which is whether it
        //! was built with glslang.
        FTK_GPU_API bool hasGLSLCompiler();

        //! Compile Vulkan GLSL to SPIR-V. Throws what the compiler says
        //! when it does not compile, or that there is no compiler.
        FTK_GPU_API std::vector<uint32_t> compileGLSL(const std::string&, ShaderStage);

        //! Get whether every shader's GLSL is compiled as it is made,
        //! whatever the device takes: asked for with the FTK_GPU_VALIDATE
        //! environment variable, to check the GLSL where there is no
        //! Vulkan to run it.
        FTK_GPU_API bool validateGLSL();

        //! Create a shader for a device from the source it takes. Throws
        //! when it cannot be made.
        FTK_GPU_API SDL_GPUShader* createShader(
            SDL_GPUDevice*,
            const ShaderSource&,
            ShaderStage,
            size_t samplers,
            size_t uniformBuffers);
    }
}
