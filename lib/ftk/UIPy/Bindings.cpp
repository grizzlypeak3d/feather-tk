// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Init.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void uiBind(py::module_& m)
        {
            m.def(
                "uiInit",
                &uiInit,
                py::arg("context"),
                "Initialize the library.");

            style(m);
            event(m);
            widgetOptions(m);
            iWidget(m);
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
            app(m);
            bellows(m);
            buttonGroup(m);
            checkBox(m);
            colorPopup(m);
            colorSwatch(m);
            colorWidget(m);
            confirmDialog(m);
            dialogSystem(m);
            divider(m);
            doubleEdit(m);
            doubleEditSlider(m);
            doubleModel(m);
            doubleSlider(m);
            fileBrowser(m);
            fileBrowserWidgets(m);
            fileEdit(m);
            floatEdit(m);
            floatEditSlider(m);
            floatModel(m);
            floatSlider(m);
            formLayout(m);
            gridLayout(m);
            groupBox(m);
            icon(m);
            iconSystem(m);
            intEdit(m);
            intEditSlider(m);
            intModel(m);
            intSlider(m);
            label(m);
            mainWindow(m);
            mdiCanvas(m);
            mdiMiniMap(m);
            mdiWidget(m);
            menu(m);
            menuBar(m);
            messageDialog(m);
            progressDialog(m);
            pushButton(m);
            radioButton(m);
            recentFilesModel(m);
            rowLayout(m);
            scrollArea(m);
            scrollWidget(m);
            splitter(m);
            stackLayout(m);
            tabBar(m);
            toolBar(m);
            tabWidget(m);
            toolButton(m);
        }
    }
}
