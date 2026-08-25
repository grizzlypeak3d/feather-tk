// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/WidgetDump.h>

#include <ftk/UI/IButton.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/ScreenshotTag.h>

namespace ftk
{
    nlohmann::json widgetDump(const std::shared_ptr<IWidget>& widget)
    {
        nlohmann::json out;
        if (!widget)
            return out;

        out["type"] = widget->getObjectName();
        const std::string tag = getScreenshotTag(widget);
        if (!tag.empty())
        {
            out["tag"] = tag;
        }
        const Box2I& g = widget->getGeometry();
        out["geometry"] = { g.min.x, g.min.y, g.w(), g.h() };
        if (!widget->isVisible(false))
        {
            out["visible"] = false;
        }
        if (widget->isClipped())
        {
            out["clipped"] = true;
        }
        if (!widget->isEnabled(false))
        {
            out["enabled"] = false;
        }
        if (widget->hasKeyFocus())
        {
            out["keyFocus"] = true;
        }

        if (auto button = std::dynamic_pointer_cast<IButton>(widget))
        {
            if (!button->getText().empty())
            {
                out["text"] = button->getText();
            }
            if (button->isChecked())
            {
                out["checked"] = true;
            }
        }
        else if (auto label = std::dynamic_pointer_cast<Label>(widget))
        {
            if (!label->getText().empty())
            {
                out["text"] = label->getText();
            }
        }
        else if (auto lineEdit = std::dynamic_pointer_cast<LineEdit>(widget))
        {
            if (!lineEdit->getText().empty())
            {
                out["text"] = lineEdit->getText();
            }
        }

        nlohmann::json children = nlohmann::json::array();
        for (const auto& child : widget->getChildren())
        {
            children.push_back(widgetDump(child));
        }
        if (!children.empty())
        {
            out["children"] = children;
        }

        return out;
    }
}
