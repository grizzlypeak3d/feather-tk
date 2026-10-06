// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/GL/Texture.h>

#include <nanobind/nanobind.h>

#include <algorithm>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void gl(nb::module_& m)
        {
            // The type of a buffer drawn into, which a viewport is asked
            // for by this name. The module is flat, so it is
            // ftk.TextureType rather than ftk.gl.TextureType. Its values
            // are named as ImageType's are, L_U8 and so on: the labels
            // have a space where that has an underscore.
            using gl::TextureType;
            using gl::getTextureTypeEnums;
            nb::enum_<TextureType> textureType(m, "TextureType");
            const std::vector<std::string> labels = gl::getTextureTypeLabels();
            for (std::size_t i = 0; i < labels.size(); ++i)
            {
                std::string label = labels[i];
                std::replace(label.begin(), label.end(), ' ', '_');
                textureType.value(enumValueName(label).c_str(), static_cast<TextureType>(i));
            }
            FTK_ENUM_BIND(m, TextureType);
            observable<TextureType>(m, "TextureType");
        }
    }
}
