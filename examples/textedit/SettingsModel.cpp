// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "SettingsModel.h"

using namespace ftk;

namespace textedit
{
    void SettingsModel::_init(
        const std::shared_ptr<Context>& context,
        float defaultDisplayScale)
    {
        _settings = Settings::create(context, getSettingsPath("feather-tk", "textedit.json"));

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

        TextEditOptions textEditOptions;
        _settings->getT("/TextEdit", textEditOptions);
        _textEditOptions = ObservableValue<TextEditOptions>::create(textEditOptions);

        TextEditModelOptions textEditModelOptions;
        _settings->getT("/TextEditModel", textEditModelOptions);
        _textEditModelOptions = ObservableValue<TextEditModelOptions>::create(textEditModelOptions);

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

        _settings->setT("/TextEdit", _textEditOptions->get());
        _settings->setT("/TextEditModel", _textEditModelOptions->get());
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

    const TextEditOptions& SettingsModel::getTextEditOptions() const
    {
        return _textEditOptions->get();
    }

    std::shared_ptr<IObservableValue<TextEditOptions> > SettingsModel::observeTextEditOptions() const
    {
        return _textEditOptions;
    }

    void SettingsModel::setTextEditOptions(const TextEditOptions& value)
    {
        _textEditOptions->setIfChanged(value);
    }

    const TextEditModelOptions& SettingsModel::getTextEditModelOptions() const
    {
        return _textEditModelOptions->get();
    }

    std::shared_ptr<IObservableValue<TextEditModelOptions> > SettingsModel::observeTextEditModelOptions() const
    {
        return _textEditModelOptions;
    }

    void SettingsModel::setTextEditModelOptions(const TextEditModelOptions& value)
    {
        _textEditModelOptions->setIfChanged(value);
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
