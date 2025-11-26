// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UIPy/Bindings.h>

#include <UIPy/Action.h>
#include <UIPy/App.h>
#include <UIPy/Bellows.h>
#include <UIPy/ButtonGroup.h>
#include <UIPy/CheckBox.h>
#include <UIPy/ColorPopup.h>
#include <UIPy/ColorSwatch.h>
#include <UIPy/ColorWidget.h>
#include <UIPy/ConfirmDialog.h>
#include <UIPy/DialogSystem.h>
#include <UIPy/Divider.h>
#include <UIPy/DoubleEdit.h>
#include <UIPy/DoubleEditSlider.h>
#include <UIPy/DoubleModel.h>
#include <UIPy/DoubleSlider.h>
#include <UIPy/Event.h>
#include <UIPy/FileBrowser.h>
#include <UIPy/FileEdit.h>
#include <UIPy/FloatEdit.h>
#include <UIPy/FloatEditSlider.h>
#include <UIPy/FloatModel.h>
#include <UIPy/FloatSlider.h>
#include <UIPy/GridLayout.h>
#include <UIPy/GroupBox.h>
#include <UIPy/IButton.h>
#include <UIPy/IDialog.h>
#include <UIPy/IMenuPopup.h>
#include <UIPy/IMouseWidget.h>
#include <UIPy/IPopup.h>
#include <UIPy/IWidget.h>
#include <UIPy/IWidgetPopup.h>
#include <UIPy/IWindow.h>
#include <UIPy/Icon.h>
#include <UIPy/IconSystem.h>
#include <UIPy/IntEdit.h>
#include <UIPy/IntEditSlider.h>
#include <UIPy/IntModel.h>
#include <UIPy/IntSlider.h>
#include <UIPy/Label.h>
#include <UIPy/MDICanvas.h>
#include <UIPy/MDIWidget.h>
#include <UIPy/MainWindow.h>
#include <UIPy/Menu.h>
#include <UIPy/MenuBar.h>
#include <UIPy/MessageDialog.h>
#include <UIPy/ProgressDialog.h>
#include <UIPy/PushButton.h>
#include <UIPy/RadioButton.h>
#include <UIPy/RecentFilesModel.h>
#include <UIPy/RowLayout.h>
#include <UIPy/ScrollArea.h>
#include <UIPy/ScrollWidget.h>
#include <UIPy/Settings.h>
#include <UIPy/Splitter.h>
#include <UIPy/Style.h>
#include <UIPy/TabBar.h>
#include <UIPy/TabWidget.h>
#include <UIPy/ToolButton.h>
#include <UIPy/WidgetOptions.h>
#include <UIPy/Window.h>

#include <ftk/UI/Init.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
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
        fileEdit(m);
        floatEdit(m);
        floatEditSlider(m);
        floatModel(m);
        floatSlider(m);
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
        tabBar(m);
        tabWidget(m);
        toolButton(m);
    }
}
