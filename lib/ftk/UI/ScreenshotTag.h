// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <memory>
#include <string>

namespace ftk
{
    namespace detail
    {
        //! Property key under which a screenshot tag is stored. Implementation
        //! detail: widgets tag themselves with setScreenshotTag() and a
        //! documentation screenshot tool reads them back with getScreenshotTag().
        const std::string screenshotKey = "Screenshot";
    }

    //! Tag a widget so a documentation screenshot tool can find it and emit its
    //! bounding box. The tag is a stable, semantic id, e.g. "MainWindow.Viewport".
    //!
    //! This lives in ftk so any widget in the stack can tag itself or its
    //! children -- including library widgets (such as the timeline) whose parts
    //! are internal and not otherwise reachable by the host application.
    //!
    //! Tagging with an empty id marks a widget as explicitly untagged (used to
    //! clear a tag when a widget's role changes, e.g. an inactive A/B button).
    inline void setScreenshotTag(
        const std::shared_ptr<IWidget>& widget,
        const std::string& id)
    {
        if (widget)
        {
            widget->setProperty(detail::screenshotKey, id);
        }
    }

    //! Returns true if the widget carries a screenshot tag (including an empty one).
    inline bool hasScreenshotTag(const std::shared_ptr<IWidget>& widget)
    {
        return widget && widget->hasProperty(detail::screenshotKey);
    }

    //! Get a widget's screenshot tag, or an empty string if it has none.
    inline std::string getScreenshotTag(const std::shared_ptr<IWidget>& widget)
    {
        return hasScreenshotTag(widget)
            ? widget->getProperty(detail::screenshotKey)
            : std::string();
    }
}
