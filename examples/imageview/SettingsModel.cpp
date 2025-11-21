// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "SettingsModel.h"

#include <ftk/Core/ImageIO.h>

using namespace ftk;

namespace imageview
{
    void SettingsModel::_init(
        const std::shared_ptr<Context>& context,
        float defaultDisplayScale)
    {
        _settings = Settings::create(context, getSettingsPath("feather-tk", "imageview.json"));

        nlohmann::json recentFiles;
        _settings->get("/RecentFiles", recentFiles);
        if (recentFiles.is_array())
        {
            for (const auto& i : recentFiles)
            {
                if (i.is_string())
                {
                    _recentFiles.push_back(i.get<std::string>());
                }
            }
        }

        FileBrowserOptions fileBrowserOptions;
        _settings->getT("/FileBrowser/Options", fileBrowserOptions);
        _fileBrowserSystem = context->getSystem<FileBrowserSystem>();
        _fileBrowserSystem->getModel()->setOptions(fileBrowserOptions);
        auto ioSystem = context->getSystem<ImageIO>();
        std::vector<std::string> exts;
        for (const auto& plugin : ioSystem->getPlugins())
        {
            const std::vector<std::string>& exts2 = plugin->getExts();
            exts.insert(exts.end(), exts2.begin(), exts2.end());
        }
        _fileBrowserSystem->getModel()->setExts(exts);
        std::string ext;
        if (!exts.empty())
        {
            ext = exts.front();
        }
        _settings->get("/FileBrowser/Ext", ext);
        _fileBrowserSystem->getModel()->setExt(ext);

        WindowSettings window;
        _settings->getT("/Window", window);
        _window = ObservableValue<WindowSettings>::create(window);

        StyleSettings style;
        style.displayScale = defaultDisplayScale;
        _settings->getT("/Style", style);
        _style = ObservableValue<StyleSettings>::create(style);
    }

    SettingsModel::~SettingsModel()
    {
        nlohmann::json recentFiles;
        for (const auto& i : _recentFiles)
        {
            recentFiles.push_back(i);
        }
        _settings->set("/RecentFiles", recentFiles);

        _settings->setT(
            "/FileBrowser/Options",
            _fileBrowserSystem->getModel()->getOptions());
        _settings->set(
            "/FileBrowser/Ext",
            _fileBrowserSystem->getModel()->getExt());

        _settings->setT("/Window", _window->get());
        _settings->setT("/Style", _style->get());
    }

    std::shared_ptr<SettingsModel> SettingsModel::create(
        const std::shared_ptr<Context>& context,
        float defaultDisplayScale)
    {
        auto out = std::shared_ptr<SettingsModel>(new SettingsModel);
        out->_init(context, defaultDisplayScale);
        return out;
    }

    const std::vector<std::filesystem::path>& SettingsModel::getRecentFiles() const
    {
        return _recentFiles;
    }

    void SettingsModel::setRecentFiles(const std::vector<std::filesystem::path>& value)
    {
        _recentFiles = value;
    }

    const WindowSettings& SettingsModel::getWindow() const
    {
        return _window->get();
    }

    std::shared_ptr<IObservableValue<WindowSettings> > SettingsModel::observeWindow() const
    {
        return _window;
    }

    void SettingsModel::setWindow(const WindowSettings& value)
    {
        _window->setIfChanged(value);
    }

    const StyleSettings& SettingsModel::getStyle() const
    {
        return _style->get();
    }

    std::shared_ptr<IObservableValue<StyleSettings> > SettingsModel::observeStyle() const
    {
        return _style;
    }

    void SettingsModel::setStyle(const StyleSettings& value)
    {
        _style->setIfChanged(value);
    }
}
