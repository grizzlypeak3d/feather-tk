// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Util.h>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <memory>

namespace ftk
{
    class Context;

    //! Get a settings path.
    FTK_API std::filesystem::path getSettingsPath(
        const std::string& directory,
        const std::string& fileName);

    //! Settings.
    class FTK_API_TYPE Settings : public std::enable_shared_from_this<Settings>
    {
    protected:
        Settings(
            const std::shared_ptr<Context>&,
            const std::filesystem::path&,
            bool reset);

    public:
        FTK_API ~Settings();

        //! Create new settings.
        FTK_API static std::shared_ptr<Settings> create(
            const std::shared_ptr<Context>&,
            const std::filesystem::path&,
            bool reset = false);

        //! Get the settings path.
        FTK_API const std::filesystem::path& getPath() const;

        //! Save the settings. The settings are also saved on destruction.
        FTK_API void save();

        //! Do the settings contain the given value?
        FTK_API bool contains(const std::string& key) const;

        //! \name Get Values
        ///@{

        FTK_API bool get(const std::string& key, nlohmann::json&) const;

        FTK_API bool get(const std::string& key, bool&);
        FTK_API bool get(const std::string& key, int&);
        FTK_API bool get(const std::string& key, int64_t&);
        FTK_API bool get(const std::string& key, float&);
        FTK_API bool get(const std::string& key, double&);
        FTK_API bool get(const std::string& key, size_t&);
        FTK_API bool get(const std::string& key, std::string&);
        FTK_API bool get(const std::string& key, std::vector<std::string>&);

        template<typename T>
        bool getT(const std::string& key, T&) const;

        ///@}

        //! \name Set Values
        ///@{

        FTK_API void set(const std::string& key, const nlohmann::json&);

        FTK_API void set(const std::string& key, bool);
        FTK_API void set(const std::string& key, int);
        FTK_API void set(const std::string& key, int64_t);
        FTK_API void set(const std::string& key, float);
        FTK_API void set(const std::string& key, double);
        FTK_API void set(const std::string& key, size_t);
        FTK_API void set(const std::string& key, const std::string&);
        FTK_API void set(const std::string& key, const std::vector<std::string>&);

        template<typename T>
        void setT(const std::string& key, const T&);

        ///@}

    private:
        FTK_PRIVATE();
    };
}

#include <ftk/UI/SettingsInline.h>
