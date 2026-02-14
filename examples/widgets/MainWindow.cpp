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

#include <ftk/UI/Divider.h>

using namespace ftk;

namespace widgets
{
    void MainWindow::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        ftk::MainWindow::_init(context, app, Size2I(1920, 1080));

        _widgets["Bellows"] = &Bellows::create;
        _widgets["Buttons"] = &Buttons::create;
        _widgets["Charts"] = &Charts::create;
        _widgets["Dialogs"] = &Dialogs::create;
        _widgets["DragDrop"] = &DragDrop::create;
        _widgets["Graphs"] = &Graphs::create;
        _widgets["Icons"] = &Icons::create;
        _widgets["Images"] = &Images::create;
        _widgets["Layouts"] = &Layouts::create;
        _widgets["Lists"] = &Lists::create;
        _widgets["MDI"] = &MDI::create;
        _widgets["Offscreen"] = &Offscreen::create;
        _widgets["Popups"] = &Popups::create;
        _widgets["Sliders"] = &Sliders::create;
        _widgets["Splitters"] = &Splitters::create;
        _widgets["Stack"] = &Stack::create;

        _tabBar = TabBar::create(context);
        for (const auto& i : _widgets)
        {
            _tabBar->addTab(i.first);
        }

        auto layout = VerticalLayout::create(context);
        layout->setSpacingRole(SizeRole::None);
        _tabBar->setParent(layout);
        Divider::create(context, Orientation::Vertical, layout);
        _layout = VerticalLayout::create(context, layout);
        _layout->setSpacingRole(SizeRole::None);
        _layout->setVStretch(Stretch::Expanding);
        setWidget(layout);

        _tabUpdate(0);

        _tabBar->setCallback(
            [this](int index)
            {
                _tabUpdate(index);
            });
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

    void MainWindow::_tabUpdate(int index)
    {
        _layout->clear();
        std::vector<std::string> tmp;
        for (const auto& i : _widgets)
        {
            tmp.push_back(i.first);
        }
        if (index >= 0 && index < tmp.size())
        {
            auto widget = _widgets[tmp[index]](getContext(), _layout);
            widget->setVStretch(Stretch::Expanding);
        }
    }
}
