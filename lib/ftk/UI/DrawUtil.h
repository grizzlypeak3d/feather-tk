// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Color.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Mesh.h>

namespace ftk
{
    //! \name Drawing
    ///@{
        
    //! Create a mesh for drawing a rectangle.
    FTK_API TriMesh2F rect(
        const Box2I&,
        int cornerRadius = 0,
        size_t resolution = 8);

    //! Create a mesh for drawing a circle.
    FTK_API TriMesh2F circle(
        const V2I&,
        int radius,
        size_t resolution = 120);

    //! Create a mesh for drawing a border.
    FTK_API TriMesh2F border(
        const Box2I&,
        int width,
        int radius = 0,
        size_t resolution = 8);

    //! Create a mesh for drawing a shadow.
    FTK_API TriMesh2F shadow(
        const Box2I&,
        int cornerRadius,
        const float alpha = .2F,
        size_t resolution = 8);
        
    ///@}
}
