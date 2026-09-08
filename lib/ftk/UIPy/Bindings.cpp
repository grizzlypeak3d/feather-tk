// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Init.h>
#include <ftk/UI/WidgetDump.h>
#include <ftk/UI/WidgetJson.h>

#include <ftk/Core/Context.h>

#include <nanobind/stl/function.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void uiBind(nb::module_& m)
        {
            m.def(
                "uiInit",
                &uiInit,
                nb::arg("context"),
                "Initialize the library.");

            m.def(
                "widgetDump",
                [](const std::shared_ptr<IWidget>& widget)
                {
                    return widgetDump(widget).dump(2);
                },
                nb::arg("widget"),
                "Serialize a widget tree as JSON for inspection.");

            m.def(
                "widgetLoad",
                [](const std::shared_ptr<Context>& context,
                    const std::string& json,
                    const std::shared_ptr<IWidget>& parent)
                {
                    auto result = widgetLoad(
                        context,
                        nlohmann::json::parse(json),
                        parent);
                    return std::make_pair(result.widget, result.errors);
                },
                nb::arg("context"),
                nb::arg("json"),
                nb::arg("parent") = nullptr,
                "Create a widget tree from JSON; returns the widget and "
                "a list of errors.");

            m.def(
                "findWidget",
                &findWidget,
                nb::arg("widget"),
                nb::arg("id"),
                "Find a widget by the id it was given in the JSON.");

            style(m);
            event(m);
            widgetOptions(m);
            iWidget(m);
            iContainer(m);
            iWindow(m);
            iMouseWidget(m);
            iPopup(m);
            iDialog(m);
            iMenuPopup(m);
            iWidgetPopup(m);
            iButton(m);
            settings(m);
            window(m);

            action(m);
            actionGroup(m);
            app(m);
            bellows(m);
            buttonGroup(m);
            checkBox(m);
            clipboardSystem(m);
            colorPopup(m);
            colorDot(m);
            colorSwatch(m);
            colorWidget(m);
            comboBox(m);
            confirmDialog(m);
            choiceDialog(m);
            inputDialog(m);
            itemButton(m);
            diagWidget(m);
            dialogSystem(m);
            divider(m);
            documentModel(m);
            doubleModel(m);
            doubleEdit(m);
            doubleEditSlider(m);
            doubleEditShuttle(m);
            doubleSlider(m);
            drivesModel(m);
            fileBrowser(m);
            fileBrowserWidgets(m);
            fileEdit(m);
            floatModel(m);
            floatEdit(m);
            floatEditSlider(m);
            floatEditShuttle(m);
            floatSlider(m);
            flowLayout(m);
            formLayout(m);
            graphWidget(m);
            gridLayout(m);
            groupBox(m);
            icon(m);
            iconSystem(m);
            imageWidget(m);
            incButtons(m);
            intModel(m);
            intEdit(m);
            intEditSlider(m);
            intEditShuttle(m);
            intSlider(m);
            // After intSlider() and floatSlider(): the color sliders
            // derive from IIntSlider and IFloatSlider, registered there.
            colorSlider(m);
            label(m);
            levelsSlider(m);
            lineEditModel(m);
            lineEdit(m);
            listItemsWidget(m);
            listWidget(m);
            mainWindow(m);
            mdiCanvas(m);
            mdiMiniMap(m);
            mdiWidget(m);
            menu(m);
            menuBar(m);
            messageDialog(m);
            overlayLayout(m);
            pieChart(m);
            progressDialog(m);
            pushButton(m);
            radioButton(m);
            recentFilesModel(m);
            rowLayout(m);
            itemButtonList(m);
            screenshotTag(m);
            scrollArea(m);
            scrollBar(m);
            scrollWidget(m);
            searchBox(m);
            shuttleWidget(m);
            spacer(m);
            splitter(m);
            splitter2D(m);
            stackLayout(m);
            svgWidget(m);
            sysLogModel(m);
            tabBar(m);
            tabWidget(m);
            textEditModel(m);
            textEdit(m);
            toolBar(m);
            toolButton(m);
            tooltip(m);
        }
    }
}
