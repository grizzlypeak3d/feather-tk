// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>

#include <ftk/Core/ISystem.h>

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
        //! the OpenGL one. Asked for by name while this is a spike: the
        //! FTK_RENDER environment variable set to "gpu".
        FTK_GPU_API bool isEnabled();

        //! Initialize the library.
        FTK_GPU_API void init(const std::shared_ptr<Context>&);
    }
}
