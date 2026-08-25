// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

#include <nlohmann/json.hpp>

namespace ftk
{
    //! \name Widgets From Data
    ///@{

    //! The result of creating widgets from JSON.
    struct WidgetLoadResult
    {
        std::shared_ptr<IWidget> widget;

        //! Problems with the data: unknown types, values that did not
        //! parse. The loader keeps going, so one mistake does not hide
        //! the rest of the interface.
        std::vector<std::string> errors;
    };

    //! Create a widget tree from JSON.
    //!
    //! The data holds the structure and the properties; behavior stays
    //! in code. Give a widget an "id" and look it up with findWidget()
    //! to attach callbacks, the way markup and script divide a web
    //! page. The vocabulary is the widget dump's, so a loaded tree
    //! dumps back out with the same names.
    //!
    //! \code
    //! {
    //!     "type": "VerticalLayout",
    //!     "marginRole": "Margin",
    //!     "children": [
    //!         { "type": "Label", "text": "Size:" },
    //!         { "type": "IntEditSlider", "id": "size",
    //!           "range": [ 1, 100 ], "value": 50 },
    //!         { "type": "PushButton", "id": "apply", "text": "Apply" }
    //!     ]
    //! }
    //! \endcode
    FTK_UI_API WidgetLoadResult widgetLoad(
        const std::shared_ptr<Context>&,
        const nlohmann::json&,
        const std::shared_ptr<IWidget>& parent = nullptr);

    //! Find a widget by the id it was given in the JSON.
    FTK_UI_API std::shared_ptr<IWidget> findWidget(
        const std::shared_ptr<IWidget>&,
        const std::string& id);

    //! A factory for one widget type: create the widget from the JSON,
    //! including any children, and report problems into the result.
    typedef std::function<std::shared_ptr<IWidget>(
        const std::shared_ptr<Context>&,
        const nlohmann::json&,
        WidgetLoadResult&)> WidgetLoadFactory;

    //! Register a widget type with the loader, so applications can make
    //! their own widgets loadable. Registering an existing name
    //! replaces it.
    FTK_UI_API void widgetLoadRegister(
        const std::string& type,
        const WidgetLoadFactory&);

    //! Create the children of a JSON node, parented to the given
    //! widget: the default handling, for factories whose widget is an
    //! ordinary container.
    FTK_UI_API void widgetLoadChildren(
        const std::shared_ptr<Context>&,
        const nlohmann::json&,
        const std::shared_ptr<IWidget>& parent,
        WidgetLoadResult&);

    ///@}
}
