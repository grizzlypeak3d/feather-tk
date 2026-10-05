// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>

#include <ftk/Core/Image.h>
#include <ftk/Core/RenderOptions.h>

struct SDL_GPUCopyPass;
struct SDL_GPUDevice;
struct SDL_GPUSampler;
struct SDL_GPUTexture;
struct SDL_GPUTransferBuffer;

namespace ftk
{
    namespace gpu
    {
        class System;

        //! Get whether an image type can be held in a texture. The types
        //! in planes are drawn from a texture for each plane, so they are
        //! not asked about here.
        FTK_GPU_API bool isTextureSupported(ImageType);

        //! Get whether a device has sixteen bit normalized textures, which
        //! Vulkan leaves to the driver. Where there are none a texture of
        //! sixteen bit integers is kept as half float instead: every driver
        //! has those, and what is drawn is the same to the eleven bits or
        //! so a half holds near one. FTK_GPU_NO_UNORM16 says there are
        //! none, to try it.
        FTK_GPU_API bool hasUNorm16(SDL_GPUDevice*);

        //! Make half floats of floats, each to the nearest: what a texture
        //! of thirty-two bit floats is kept as where a device does not
        //! filter those, which Vulkan also leaves to the driver. See
        //! hasFloatFilter(). A value too large for a half is the largest a
        //! half holds, rather than infinity.
        FTK_GPU_API void floatToHalf(const float*, uint16_t*, size_t count);

        //! Texture options.
        struct FTK_GPU_API_TYPE TextureOptions
        {
            ImageFilters filters;

            bool operator == (const TextureOptions&) const = default;
        };

        //! Texture.
        //!
        //! There are no three channel textures: an RGB image is given a
        //! fourth channel as it is copied.
        class FTK_GPU_API_TYPE Texture : public std::enable_shared_from_this<Texture>
        {
            FTK_NON_COPYABLE(Texture);

        protected:
            void _init(
                const std::shared_ptr<System>&,
                const ImageInfo&,
                const TextureOptions&);

            Texture();

        public:
            FTK_GPU_API ~Texture();

            //! Create a new texture.
            FTK_GPU_API static std::shared_ptr<Texture> create(
                const std::shared_ptr<System>&,
                const ImageInfo&,
                const TextureOptions& = TextureOptions());

            //! Get the image information.
            FTK_GPU_API const ImageInfo& getInfo() const;

            //! Get the texture.
            FTK_GPU_API SDL_GPUTexture* getTexture() const;

            //! Get the sampler.
            FTK_GPU_API SDL_GPUSampler* getSampler() const;

            //! Copy an image to the whole texture. What the texture held is
            //! not kept, which lets a texture that an earlier draw of this
            //! frame is waiting on be given new contents without changing
            //! what that draw sees.
            //!
            //! The copies are sent in a command buffer of their own, now.
            //! A renderer that is drawing a frame sends its own instead,
            //! with prepare() and send(), in the command buffer that draws
            //! with them.
            FTK_GPU_API void copy(const std::shared_ptr<Image>&);

            //! Copy image data to the whole texture.
            FTK_GPU_API void copy(const uint8_t*, const ImageInfo&);

            //! Copy an image to part of the texture, keeping the rest.
            FTK_GPU_API void copy(const std::shared_ptr<Image>&, int x, int y);

            //! A copy that has been made ready and not yet sent: the data,
            //! laid out as the texture keeps it, and where it goes.
            struct Upload
            {
                SDL_GPUTransferBuffer* transfer = nullptr;
                SDL_GPUTexture* texture = nullptr;
                int x = 0;
                int y = 0;
                int w = 0;
                int h = 0;
                //! The whole texture, whose old contents are not kept.
                bool whole = false;
                //! Whether the transfer buffer is this copy's own, let go
                //! of once sent, rather than the texture's, which is kept
                //! for the next copy of the whole texture.
                bool owned = false;
            };

            //! Make a copy of an image to the whole texture ready. Nothing
            //! is made where the image is not one the texture can hold.
            //!
            //! The data goes through a transfer buffer the texture keeps
            //! from one copy to the next, rather than one made each time:
            //! a picture playing sent three planes a frame, each through a
            //! buffer made and let go of for it. A copy the device has not
            //! finished with keeps the memory it was given; the next is
            //! given other memory. Draw with the texture before making the
            //! next copy ready.
            FTK_GPU_API Upload prepare(const std::shared_ptr<Image>&);

            //! Make a copy of image data to the whole texture ready.
            FTK_GPU_API Upload prepare(const uint8_t*, const ImageInfo&);

            //! Make a copy of an image to part of the texture ready.
            FTK_GPU_API Upload prepare(const std::shared_ptr<Image>&, int x, int y);

            //! Send a copy that was made ready, in a copy pass, and let go
            //! of its data.
            FTK_GPU_API static void send(SDL_GPUDevice*, SDL_GPUCopyPass*, const Upload&);

            //! Let go of a copy that was made ready and will not be sent.
            FTK_GPU_API static void discard(SDL_GPUDevice*, const Upload&);

            //! Get the number of textures that exist.
            FTK_GPU_API static size_t getObjectCount();

            //! Get the bytes the textures that exist take, on the GPU.
            FTK_GPU_API static size_t getTotalByteCount();

        private:
            Upload _prepare(const uint8_t*, const ImageInfo&, int x, int y, bool whole);
            void _copy(const Upload&);

            FTK_PRIVATE();
        };
    }
}
