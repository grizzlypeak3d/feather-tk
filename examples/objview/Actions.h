// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include "Settings.h"

#include <ftk/UI/Action.h>
#include <ftk/UI/DocumentModel.h>

namespace objview
{
    class App;
    class MainWindow;

    //! This class provides actions that are used to populate the menus
    //! and tool bars.
    class Actions : public std::enable_shared_from_this<Actions>
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<MainWindow>&);

        Actions() = default;

    public:
        virtual ~Actions();

        //! Create new actions.
        static std::shared_ptr<Actions> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<MainWindow>&);

        //! Get the actions.
        const std::map<std::string, std::shared_ptr<ftk::Action> > getActions() const;

        //! Get an action by name.
        std::shared_ptr<ftk::Action> getAction(const std::string&) const;

    private:
        void _createFileActions(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&);
        void _createEditActions(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&);
        void _createWindowActions(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<MainWindow>&);
        void _createViewActions(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<MainWindow>&);
        void _createRenderActions(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<MainWindow>&);

        std::map<std::string, std::shared_ptr<ftk::Action> > _actions;
        std::map<RenderMode, std::shared_ptr<ftk::Action> > _renderModeActions;

        std::shared_ptr<ftk::Observer<std::shared_ptr<ftk::IDocument> > > _currentObserver;
        std::shared_ptr<ftk::Observer<WindowSettings> > _windowSettingsObserver;
        std::shared_ptr<ftk::Observer<bool> > _fullScreenObserver;
        std::shared_ptr<ftk::Observer<RenderSettings> > _renderSettingsObserver;
    };
}
