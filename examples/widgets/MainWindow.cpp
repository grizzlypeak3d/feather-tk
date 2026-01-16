// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "MainWindow.h"

#include "Bellows.h"
#include "Buttons.h"
#include "Charts.h"
#include "Dialogs.h"
#include "DragDrop.h"
#include "Graphs.h"
#include "Icons.h"
#include "Images.h"
#include "Layouts.h"
#include "Lists.h"
#include "MDI.h"
#include "Offscreen.h"
#include "Popups.h"
#include "Sliders.h"
#include "Splitters.h"
#include "Stack.h"

using namespace ftk;

namespace widgets
{
    void MainWindow::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        ftk::MainWindow::_init(context, app, Size2I(1920, 1080));

        _tabWidget = TabWidget::create(context);
        _tabWidget->addTab("Bellows", Bellows::create(context));
        _tabWidget->addTab("Buttons", Buttons::create(context));
        _tabWidget->addTab("Charts", Charts::create(context));
        _tabWidget->addTab("Dialogs", Dialogs::create(context));
        _tabWidget->addTab("DragDrop", DragDrop::create(context));
        _tabWidget->addTab("Graphs", Graphs::create(context));
        _tabWidget->addTab("Icons", Icons::create(context));
        _tabWidget->addTab("Images", Images::create(context));
        _tabWidget->addTab("Layouts", Layouts::create(context));
        _tabWidget->addTab("Lists", Lists::create(context));
        _tabWidget->addTab("MDI", MDI::create(context));
        _tabWidget->addTab("Offscreen", Offscreen::create(context));
        _tabWidget->addTab("Popups", Popups::create(context, getMenuBar()));
        _tabWidget->addTab("Sliders", Sliders::create(context));
        _tabWidget->addTab("Splitters", Splitters::create(context));
        _tabWidget->addTab("Stack", Stack::create(context));
        setWidget(_tabWidget);
    }

    MainWindow::~MainWindow()
    {}

    std::shared_ptr<MainWindow> MainWindow::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        auto out = std::shared_ptr<MainWindow>(new MainWindow);
        out->_init(context, app);
        return out;
    }
}
