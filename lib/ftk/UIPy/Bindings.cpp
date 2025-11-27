// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/Action.h>
#include <ftk/UIPy/App.h>
#include <ftk/UIPy/Bellows.h>
#include <ftk/UIPy/ButtonGroup.h>
#include <ftk/UIPy/CheckBox.h>
#include <ftk/UIPy/ColorPopup.h>
#include <ftk/UIPy/ColorSwatch.h>
#include <ftk/UIPy/ColorWidget.h>
#include <ftk/UIPy/ConfirmDialog.h>
#include <ftk/UIPy/DialogSystem.h>
#include <ftk/UIPy/Divider.h>
#include <ftk/UIPy/DoubleEdit.h>
#include <ftk/UIPy/DoubleEditSlider.h>
#include <ftk/UIPy/DoubleModel.h>
#include <ftk/UIPy/DoubleSlider.h>
#include <ftk/UIPy/Event.h>
#include <ftk/UIPy/FileBrowser.h>
#include <ftk/UIPy/FileEdit.h>
#include <ftk/UIPy/FloatEdit.h>
#include <ftk/UIPy/FloatEditSlider.h>
#include <ftk/UIPy/FloatModel.h>
#include <ftk/UIPy/FloatSlider.h>
#include <ftk/UIPy/GridLayout.h>
#include <ftk/UIPy/GroupBox.h>
#include <ftk/UIPy/IButton.h>
#include <ftk/UIPy/IDialog.h>
#include <ftk/UIPy/IMenuPopup.h>
#include <ftk/UIPy/IMouseWidget.h>
#include <ftk/UIPy/IPopup.h>
#include <ftk/UIPy/IWidget.h>
#include <ftk/UIPy/IWidgetPopup.h>
#include <ftk/UIPy/IWindow.h>
#include <ftk/UIPy/Icon.h>
#include <ftk/UIPy/IconSystem.h>
#include <ftk/UIPy/IntEdit.h>
#include <ftk/UIPy/IntEditSlider.h>
#include <ftk/UIPy/IntModel.h>
#include <ftk/UIPy/IntSlider.h>
#include <ftk/UIPy/Label.h>
#include <ftk/UIPy/MDICanvas.h>
#include <ftk/UIPy/MDIWidget.h>
#include <ftk/UIPy/MainWindow.h>
#include <ftk/UIPy/Menu.h>
#include <ftk/UIPy/MenuBar.h>
#include <ftk/UIPy/MessageDialog.h>
#include <ftk/UIPy/ProgressDialog.h>
#include <ftk/UIPy/PushButton.h>
#include <ftk/UIPy/RadioButton.h>
#include <ftk/UIPy/RecentFilesModel.h>
#include <ftk/UIPy/RowLayout.h>
#include <ftk/UIPy/ScrollArea.h>
#include <ftk/UIPy/ScrollWidget.h>
#include <ftk/UIPy/Settings.h>
#include <ftk/UIPy/Splitter.h>
#include <ftk/UIPy/Style.h>
#include <ftk/UIPy/TabBar.h>
#include <ftk/UIPy/ToolBar.h>
#include <ftk/UIPy/TabWidget.h>
#include <ftk/UIPy/ToolButton.h>
#include <ftk/UIPy/WidgetOptions.h>
#include <ftk/UIPy/Window.h>

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
            toolBar(m);
            tabWidget(m);
            toolButton(m);
        }
    }
}
