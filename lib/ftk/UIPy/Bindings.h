// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <pybind11/pybind11.h>

namespace ftk
{
    namespace python
    {
        FTK_API void action(pybind11::module_&);
        FTK_API void app(pybind11::module_&);
        FTK_API void bellows(pybind11::module_&);
        FTK_API void buttonGroup(pybind11::module_&);
        FTK_API void checkBox(pybind11::module_&);
        FTK_API void colorPopup(pybind11::module_&);
        FTK_API void colorSwatch(pybind11::module_&);
        FTK_API void colorWidget(pybind11::module_&);
        FTK_API void confirmDialog(pybind11::module_&);
        FTK_API void dialogSystem(pybind11::module_&);
        FTK_API void divider(pybind11::module_&);
        FTK_API void doubleEdit(pybind11::module_&);
        FTK_API void doubleEditSlider(pybind11::module_&);
        FTK_API void doubleModel(pybind11::module_&);
        FTK_API void doubleSlider(pybind11::module_&);
        FTK_API void event(pybind11::module_&);
        FTK_API void fileBrowser(pybind11::module_&);
        FTK_API void fileBrowserWidgets(pybind11::module_&);
        FTK_API void fileEdit(pybind11::module_&);
        FTK_API void floatEdit(pybind11::module_&);
        FTK_API void floatEditSlider(pybind11::module_&);
        FTK_API void floatModel(pybind11::module_&);
        FTK_API void floatSlider(pybind11::module_&);
        FTK_API void formLayout(pybind11::module_&);
        FTK_API void gridLayout(pybind11::module_&);
        FTK_API void groupBox(pybind11::module_&);
        FTK_API void iButton(pybind11::module_&);
        FTK_API void icon(pybind11::module_&);
        FTK_API void iconSystem(pybind11::module_&);
        FTK_API void iDialog(pybind11::module_&);
        FTK_API void iMenuPopup(pybind11::module_&);
        FTK_API void iMouseWidget(pybind11::module_&);
        FTK_API void intEdit(pybind11::module_&);
        FTK_API void intEditSlider(pybind11::module_&);
        FTK_API void intModel(pybind11::module_&);
        FTK_API void intSlider(pybind11::module_&);
        FTK_API void iPopup(pybind11::module_&);
        FTK_API void iWidget(pybind11::module_&);
        FTK_API void iWidgetPopup(pybind11::module_&);
        FTK_API void iWindow(pybind11::module_&);
        FTK_API void label(pybind11::module_&);
        FTK_API void mainWindow(pybind11::module_&);
        FTK_API void mdiCanvas(pybind11::module_&);
        FTK_API void mdiMiniMap(pybind11::module_&);
        FTK_API void mdiWidget(pybind11::module_&);
        FTK_API void menu(pybind11::module_&);
        FTK_API void menuBar(pybind11::module_&);
        FTK_API void messageDialog(pybind11::module_&);
        FTK_API void progressDialog(pybind11::module_&);
        FTK_API void pushButton(pybind11::module_&);
        FTK_API void radioButton(pybind11::module_&);
        FTK_API void recentFilesModel(pybind11::module_&);
        FTK_API void rowLayout(pybind11::module_&);
        FTK_API void scrollArea(pybind11::module_&);
        FTK_API void scrollWidget(pybind11::module_&);
        FTK_API void settings(pybind11::module_&);
        FTK_API void splitter(pybind11::module_&);
        FTK_API void stackLayout(pybind11::module_&);
        FTK_API void style(pybind11::module_&);
        FTK_API void tabBar(pybind11::module_&);
        FTK_API void tabWidget(pybind11::module_&);
        FTK_API void toolBar(pybind11::module_&);
        FTK_API void toolButton(pybind11::module_&);
        FTK_API void widgetOptions(pybind11::module_&);
        FTK_API void widgetOptions(pybind11::module_&);
        FTK_API void window(pybind11::module_&);

        FTK_API void uiBind(pybind11::module_&);
    }
}
