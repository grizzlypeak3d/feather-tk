// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>

#include <ftk/Core/ISystem.h>

#include <string>
#include <utility>
#include <vector>

struct SDL_GPUDevice;
struct SDL_GPUTexture;

namespace ftk
{
    class IRenderFactory;

    //! Rendering with SDL's GPU API: Metal on macOS, Vulkan elsewhere.
    //!
    //! A spike. This draws what the OpenGL renderer draws, through the same
    //! IRender, so that the two can be put side by side.
    namespace gpu
    {
        //! GPU system.
        class FTK_GPU_API_TYPE System : public ISystem
        {
        protected:
            System(const std::shared_ptr<Context>&);

        public:
            FTK_GPU_API virtual ~System();

            //! Create a new system.
            FTK_GPU_API static std::shared_ptr<System> create(const std::shared_ptr<Context>&);

            //! Get the device, which is made the first time it is asked
            //! for. Throws when there is none to be had.
            FTK_GPU_API SDL_GPUDevice* getDevice();

            //! Get the name of the driver the device uses (e.g., "metal",
            //! "vulkan").
            FTK_GPU_API std::string getDriver();

            //! Get what the device says of itself, for a report: the
            //! driver SDL draws through, and the name and the system's
            //! driver where they are told.
            FTK_GPU_API std::vector<std::pair<std::string, std::string> > getInfo();

            //! Get the render factory.
            FTK_GPU_API const std::shared_ptr<IRenderFactory>& getRenderFactory() const;

            //! Set the render factory.
            FTK_GPU_API void setRenderFactory(const std::shared_ptr<IRenderFactory>&);

            //! \name Texture IDs
            //! IRender::drawTexture() takes a number, which is what OpenGL
            //! calls a texture. These give a texture one.
            ///@{

            FTK_GPU_API unsigned int addTexture(SDL_GPUTexture*);
            FTK_GPU_API void removeTexture(unsigned int);
            FTK_GPU_API SDL_GPUTexture* getTexture(unsigned int) const;

            ///@}

        private:
            FTK_PRIVATE();
        };

        //! Get whether windows are drawn with this renderer rather than
        //! the OpenGL one. They are only where it is asked for: by an
        //! application's setting or command line, or by the FTK_RENDER
        //! environment variable saying "gpu".
        //!
        //! It is decided by start(), and is false before then.
        FTK_GPU_API bool isEnabled();

        //! Get whether the FTK_RENDER environment variable asks for this
        //! renderer, for a program with no setting of its own to ask.
        FTK_GPU_API bool isRequested();

        //! Initialize the library. Nothing is started: see start().
        FTK_GPU_API void init(const std::shared_ptr<Context>&);

        //! Start this renderer, for a program that was asked to draw with
        //! it, and get whether it draws. It does only where a device can
        //! be made for it, and finding that out is making one: where none
        //! can, on a machine with no driver for Vulkan say, that is logged
        //! and the OpenGL renderer draws.
        //!
        //! Apart from init(), as the OpenGL system's is: making a device
        //! starts SDL's video subsystem, which wants a display, and an
        //! application that only prints its help has no use for either.
        //! An application calls this once it knows it is going to run,
        //! before its first window, and after the first time it does
        //! nothing.
        FTK_GPU_API bool start(const std::shared_ptr<Context>&);
    }
}
