// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include "Settings.h"

#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/Settings.h>

namespace objview
{
    //! Global settings that are save to file.
    class SettingsModel : public std::enable_shared_from_this<SettingsModel>
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            float defaultDisplayScale);

        SettingsModel() = default;

    public:
        virtual ~SettingsModel();

        //! Create a new settings model.
        static std::shared_ptr<SettingsModel> create(
            const std::shared_ptr<ftk::Context>&,
            float defaultDisplayScale);

        //! \name Recent Files
        ///@{

        const std::vector<std::filesystem::path>& getRecentFiles() const;
        void setRecentFiles(const std::vector<std::filesystem::path>&);

        ///@}

        //! \name Window
        ///@{

        const WindowSettings& getWindow() const;
        std::shared_ptr<ftk::IObservable<WindowSettings> > observeWindow() const;
        void setWindow(const WindowSettings&);

        ///@}

        //! \name Rendering
        ///@{

        const RenderSettings& getRender() const;
        std::shared_ptr<ftk::IObservable<RenderSettings> > observeRender() const;
        void setRender(const RenderSettings&);

        ///@}

        //! \name Style
        ///@{

        const StyleSettings& getStyle() const;
        std::shared_ptr<ftk::IObservable<StyleSettings> > observeStyle() const;
        void setStyle(const StyleSettings&);

        ///@}

    private:
        std::shared_ptr<ftk::Settings> _settings;
        std::vector<std::filesystem::path> _recentFiles;
        std::shared_ptr<ftk::FileBrowserSystem> _fileBrowserSystem;
        std::shared_ptr<ftk::Observable<WindowSettings> > _window;
        std::shared_ptr<ftk::Observable<RenderSettings> > _render;
        std::shared_ptr<ftk::Observable<StyleSettings> > _style;
    };
}
