// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

#include <nlohmann/json.hpp>

namespace ftk
{
    //! \name Debugging
    ///@{

    //! Serialize a widget tree for inspection: the type, screenshot tag,
    //! geometry, and state of every widget, with the text where a widget
    //! has some. State appears only when it differs from the common case
    //! -- visible, unclipped, enabled, unfocused -- so a large tree stays
    //! readable.
    FTK_UI_API nlohmann::json widgetDump(const std::shared_ptr<IWidget>&);

    ///@}
}
