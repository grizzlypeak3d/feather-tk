// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/Core/Color.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Mesh.h>

#include <array>

namespace ftk
{
    //! \name Drawing
    ///@{
        
    //! Create a mesh for drawing a rectangle.
    FTK_UI_API TriMesh2F rect(
        const Box2I&,
        int cornerRadius = 0,
        size_t resolution = 16);

    //! Create a mesh for drawing a rectangle with a radius for each corner,
    //! in the order top left, top right, bottom right, bottom left. A zero
    //! radius is a square corner.
    FTK_UI_API TriMesh2F rect(
        const Box2I&,
        const std::array<int, 4>& cornerRadii,
        size_t resolution = 16);

    //! Create a mesh for drawing a circle.
    FTK_UI_API TriMesh2F circle(
        const V2I&,
        int radius,
        size_t resolution = 16);

    //! Create a mesh for drawing a border.
    FTK_UI_API TriMesh2F border(
        const Box2I&,
        int width,
        int radius = 0,
        size_t resolution = 16);

    //! Create a mesh for drawing a border with a radius for each corner, in
    //! the order top left, top right, bottom right, bottom left. A zero
    //! radius is a square corner.
    FTK_UI_API TriMesh2F border(
        const Box2I&,
        int width,
        const std::array<int, 4>& radii,
        size_t resolution = 16);

    //! Get the tint drawn over something checked or selected: the checked
    //! color, faint. A filled row or tile outweighs the picture beside it.
    FTK_UI_API Color4F checkedTint(const Color4F& checked);

    //! Get the color for the icon and text of something checked: the
    //! checked color brightened if it is dark and deepened if it is light,
    //! so that it reads against the tint.
    FTK_UI_API Color4F checkedHighlight(const Color4F& checked);

    //! Create a mesh for drawing a shadow.
    FTK_UI_API TriMesh2F shadow(
        const Box2I&,
        int cornerRadius,
        const float alpha = .2F,
        size_t resolution = 16);
        
    ///@}
}
