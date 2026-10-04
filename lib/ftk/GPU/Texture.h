// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>

#include <ftk/Core/Image.h>
#include <ftk/Core/RenderOptions.h>

struct SDL_GPUSampler;
struct SDL_GPUTexture;

namespace ftk
{
    namespace gpu
    {
        class System;

        //! Get whether an image type can be held in a texture. The types
        //! in planes are drawn from a texture for each plane, so they are
        //! not asked about here.
        FTK_GPU_API bool isTextureSupported(ImageType);

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
            FTK_GPU_API void copy(const std::shared_ptr<Image>&);

            //! Copy image data to the whole texture.
            FTK_GPU_API void copy(const uint8_t*, const ImageInfo&);

            //! Copy an image to part of the texture, keeping the rest.
            FTK_GPU_API void copy(const std::shared_ptr<Image>&, int x, int y);

        private:
            void _copy(const uint8_t*, const ImageInfo&, int x, int y, bool cycle);

            FTK_PRIVATE();
        };
    }
}
