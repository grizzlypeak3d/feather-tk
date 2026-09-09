// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Settings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/FileIO.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/Path.h>
#include <ftk/Core/String.h>

#include <map>

namespace ftk
{
    std::filesystem::path getSettingsPath(
        const std::string& directory,
        const std::string& fileName)
    {
        return getUserPath(UserPath::Config) / directory / fileName;
    }

    struct Settings::Private
    {
        std::weak_ptr<LogSystem> logSystem;
        std::filesystem::path path;
        nlohmann::json settings;

        // The file was there and could not be opened. It is left as it is:
        // it may well be intact and merely busy, and writing the defaults
        // over it on the way out would turn a failed launch into lost
        // settings. A file that opened and would not parse is set aside
        // under another name (see the constructor) and replaced.
        bool preserve = false;
    };

    Settings::Settings(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path,
        bool reset) :
        _p(new Private)
    {
        FTK_P();

        auto logSystem = context->getLogSystem();
        p.logSystem = logSystem;

        p.path = path;

        if (!p.path.empty())
        {
            // Reading the settings must not be able to stop the application.
            // The file can be held by a scanner or a sync client at the
            // moment it is opened -- the same way the log was, before that
            // was guarded -- and an exception leaving here leaves main().
            bool opened = false;
            try
            {
                if (p.path.has_parent_path())
                {
                    std::filesystem::create_directories(p.path.parent_path());
                }
                if (std::filesystem::exists(p.path) && !reset)
                {
                    const std::string contents = read(FileIO::create(p.path, FileMode::Read));
                    opened = true;
                    p.settings = nlohmann::json::parse(contents);
                }
            }
            catch (const std::exception& e)
            {
                p.preserve = !opened;
                logSystem->print(
                    "ftk::Settings",
                    Format("Cannot read settings: {0}: {1}").arg(p.path).arg(e.what()),
                    LogType::Error);
                if (opened)
                {
                    // Kept rather than overwritten with the defaults at
                    // exit: the settings can be got back from it by hand.
                    try
                    {
                        std::filesystem::path bad = p.path;
                        bad += ".bad";
                        std::filesystem::rename(p.path, bad);
                        logSystem->print(
                            "ftk::Settings",
                            Format("Settings set aside: {0}").arg(bad),
                            LogType::Warning);
                    }
                    catch (const std::exception&)
                    {}
                }
            }
        }
    }

    Settings::~Settings()
    {
        save();
    }

    std::shared_ptr<Settings> Settings::create(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path,
        bool reset)
    {
        return std::shared_ptr<Settings>(new Settings(context, path, reset));
    }

    const std::filesystem::path& Settings::getPath() const
    {
        return _p->path;
    }

    void Settings::save()
    {
        FTK_P();
        if (!p.path.empty() && !p.preserve)
        {
            try
            {
                // Written whole to a file of its own and then renamed into
                // place, so the settings file is never partly written:
                // another instance starting as this one quits read a
                // truncated file, took the defaults, and wrote them back
                // over the settings at its own exit.
                std::filesystem::path tmp = p.path;
                tmp += ".tmp";
                FileIO::create(tmp, FileMode::Write)->write(p.settings.dump(4));
                std::filesystem::rename(tmp, p.path);
            }
            catch (const std::exception& e)
            {
                if (auto logSystem = p.logSystem.lock())
                {
                    logSystem->print(
                        "ftk::Settings",
                        Format("Cannot write settings: {0}: {1}").arg(p.path).arg(e.what()),
                        LogType::Error);
                }
            }
        }
    }

    bool Settings::contains(const std::string& key) const
    {
        FTK_P();
        return p.settings.contains(nlohmann::json::json_pointer(key));
    }

    bool Settings::get(const std::string& key, nlohmann::json& value) const
    {
        FTK_P();
        bool out = false;
        try
        {
            value = p.settings.at(nlohmann::json::json_pointer(key));
            out = true;
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, bool& value)
    {
        FTK_P();
        bool out = false;
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            if (i.is_boolean())
            {
                value = i;
                out = true;
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, int& value)
    {
        FTK_P();
        bool out = false;
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            if (i.is_number_integer())
            {
                value = i;
                out = true;
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, int64_t& value)
    {
        bool out = false;
        FTK_P();
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            if (i.is_number_integer())
            {
                value = i;
                out = true;
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, float& value)
    {
        FTK_P();
        bool out = false;
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            if (i.is_number_float())
            {
                value = i;
                out = true;
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, double& value)
    {
        FTK_P();
        bool out = false;
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            if (i.is_number_float())
            {
                value = i;
                out = true;
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, size_t& value)
    {
        FTK_P();
        bool out = false;
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            if (i.is_number_unsigned())
            {
                value = i;
                out = true;
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, std::string& value)
    {
        FTK_P();
        bool out = false;
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            if (i.is_string())
            {
                value = i;
                out = true;
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool Settings::get(const std::string& key, std::vector<std::string>& value)
    {
        FTK_P();
        bool out = false;
        value.clear();
        try
        {
            auto i = p.settings.at(nlohmann::json::json_pointer(key));
            for (auto j = i.begin(); j != i.end(); ++j)
            {
                if (j->is_string())
                {
                    value.push_back(*j);
                    out = true;
                }
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    void Settings::set(const std::string& key, const nlohmann::json& value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, bool value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, int value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, int64_t value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, float value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, double value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, size_t value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, const std::string& value)
    {
        try
        {
            _p->settings[nlohmann::json::json_pointer(key)] = value;
        }
        catch (const std::exception&)
        {}
    }

    void Settings::set(const std::string& key, const std::vector<std::string>& value)
    {
        try
        {
            nlohmann::json json;
            for (const auto& i : value)
            {
                json.push_back(i);
            }
            _p->settings[nlohmann::json::json_pointer(key)] = json;
        }
        catch (const std::exception&)
        {}
    }
}
