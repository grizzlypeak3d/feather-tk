// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Box.h>
#include <ftk/Core/Util.h>

#include <memory>

namespace ftk
{
    class IRender;

    //! \name Rendering
    ///@{
        
    //! Set and restore the render size.
    class FTK_API_TYPE RenderSizeState
    {
    public:
        FTK_API RenderSizeState(const std::shared_ptr<IRender>&);

        FTK_API ~RenderSizeState();

    private:
        FTK_PRIVATE();
    };

    //! Set and restore the viewport.
    class FTK_API_TYPE ViewportState
    {
    public:
        FTK_API ViewportState(const std::shared_ptr<IRender>&);

        FTK_API ~ViewportState();

    private:
        FTK_PRIVATE();
    };

    //! Set and restore whether the clipping rectangle is enabled.
    class FTK_API_TYPE ClipRectEnabledState
    {
    public:
        FTK_API ClipRectEnabledState(const std::shared_ptr<IRender>&);

        FTK_API ~ClipRectEnabledState();

    private:
        FTK_PRIVATE();
    };

    //! Set and restore the clipping rectangle.
    class FTK_API_TYPE ClipRectState
    {
    public:
        FTK_API ClipRectState(const std::shared_ptr<IRender>&);

        FTK_API ~ClipRectState();

        FTK_API const Box2I& getClipRect() const;

    private:
        FTK_PRIVATE();
    };

    //! Set and restore the transform.
    class FTK_API_TYPE TransformState
    {
    public:
        FTK_API TransformState(const std::shared_ptr<IRender>&);

        FTK_API ~TransformState();

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
