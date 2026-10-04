// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/Box.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Util.h>

#include <memory>

namespace ftk
{
    class IRender;

    //! \name Rendering
    ///@{
        
    //! Set and restore the render size.
    class FTK_CORE_API_TYPE RenderSizeState
    {
    public:
        FTK_CORE_API RenderSizeState(const std::shared_ptr<IRender>&);

        FTK_CORE_API ~RenderSizeState();

    private:
        FTK_PRIVATE();
    };

    //! Set and restore the viewport.
    class FTK_CORE_API_TYPE ViewportState
    {
    public:
        FTK_CORE_API ViewportState(const std::shared_ptr<IRender>&);

        FTK_CORE_API ~ViewportState();

    private:
        FTK_PRIVATE();
    };

    //! Set and restore whether the clipping rectangle is enabled.
    class FTK_CORE_API_TYPE ClipRectEnabledState
    {
    public:
        FTK_CORE_API ClipRectEnabledState(const std::shared_ptr<IRender>&);

        FTK_CORE_API ~ClipRectEnabledState();

    private:
        FTK_PRIVATE();
    };

    //! Set and restore the clipping rectangle.
    class FTK_CORE_API_TYPE ClipRectState
    {
    public:
        FTK_CORE_API ClipRectState(const std::shared_ptr<IRender>&);

        FTK_CORE_API ~ClipRectState();

        FTK_CORE_API const Box2I& getClipRect() const;

    private:
        FTK_PRIVATE();
    };

    //! Set and restore the transform.
    class FTK_CORE_API_TYPE TransformState
    {
    public:
        FTK_CORE_API TransformState(const std::shared_ptr<IRender>&);

        FTK_CORE_API ~TransformState();

    private:
        FTK_PRIVATE();
    };
        
    //! Get the table a separable resample of one axis is weighed with: for
    //! each output pixel, the source coordinate and weight of every tap,
    //! as an ImageType::LA_F32 image with a column for each output pixel
    //! and a row for each tap. Reducing is weighed with a Lanczos kernel
    //! and enlarging with Mitchell-Netravali.
    FTK_CORE_API std::shared_ptr<Image> getScaleContrib(int in, int out, int& taps);

    ///@}
}
