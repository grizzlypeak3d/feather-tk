// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>

#include <ftk/Core/Util.h>

#include <memory>
#include <string>

struct SDL_GPUCommandBuffer;
struct SDL_GPUTexture;
struct SDL_Window;

namespace ftk
{
    namespace gpu
    {
        class System;

        //! What a window's swapchain holds, and so how what is drawn has
        //! to be written into it.
        enum class FTK_GPU_API_TYPE Composition
        {
            //! Eight bits, display encoded: what was drawn, as it is.
            SDR,
            //! Half float, linear, where one is the white of the user
            //! interface and more than one is brighter than that.
            HDRExtendedLinear,
            //! Ten bits, Rec. 2020 primaries, PQ encoded.
            HDR10,

            Count,
            First = SDR
        };

        //! Get a composition's label.
        FTK_GPU_API std::string getLabel(Composition);

        //! Get the composition asked for with the FTK_GPU_SWAPCHAIN
        //! environment variable: "sdr", "hdr" or "hdr10". Without it the
        //! swapchain follows the display; see getComposition().
        FTK_GPU_API Composition getCompositionRequest();

        //! Get whether a composition was asked for with FTK_GPU_SWAPCHAIN.
        FTK_GPU_API bool hasCompositionRequest();

        //! Get the composition that suits the display a window is on: HDR
        //! where the display is showing HDR, and SDR where it is not.
        //! Extended linear is taken where the window can have it and HDR10
        //! where it cannot, which is Vulkan on Wayland.
        FTK_GPU_API Composition getComposition(
            const std::shared_ptr<System>&,
            SDL_Window*);

        //! Set a window's swapchain to a composition, or the nearest to it
        //! the window supports. Returns what it was set to.
        FTK_GPU_API Composition setComposition(
            const std::shared_ptr<System>&,
            SDL_Window*,
            Composition);

        //! Get where SDR white is in a window's swapchain, as a multiple of
        //! 80 nits.
        FTK_GPU_API float getSDRWhiteLevel(SDL_Window*, Composition);

        //! Draws what a renderer drew into what a window shows.
        //!
        //! What is drawn is display encoded -- the user interface's colors
        //! are sRGB, with one as white -- and values above one are brighter
        //! than white by the same curve. A swapchain that holds something
        //! else is written in its terms: decoded to linear light, and for
        //! HDR10 taken to Rec. 2020 and PQ encoded, with white where the
        //! system says its SDR white is.
        class FTK_GPU_API_TYPE Present
        {
            FTK_NON_COPYABLE(Present);

        protected:
            Present();

        public:
            FTK_GPU_API ~Present();

            //! Create a new presenter.
            FTK_GPU_API static std::shared_ptr<Present> create(const std::shared_ptr<System>&);

            //! Draw a texture over the whole of another. The format is
            //! the destination's SDL_GPUTextureFormat, and the white level
            //! is that of SDR white, as a multiple of 80 nits.
            FTK_GPU_API void draw(
                SDL_GPUCommandBuffer*,
                SDL_GPUTexture* source,
                SDL_GPUTexture* destination,
                int destinationFormat,
                Composition,
                float sdrWhiteLevel = 1.F);

        private:
            FTK_PRIVATE();
        };
    }
}
