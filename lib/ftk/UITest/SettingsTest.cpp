// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/SettingsTest.h>

#include <ftk/UI/Settings.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/FileIO.h>

#include <filesystem>

namespace ftk
{
    namespace ui_test
    {
        SettingsTest::SettingsTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::SettingsTest")
        {}

        SettingsTest::~SettingsTest()
        {}

        std::shared_ptr<SettingsTest> SettingsTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<SettingsTest>(new SettingsTest(context));
        }

        void SettingsTest::run()
        {
            const std::filesystem::path path = _getTempDir() / "SettingsTest.json";
            std::filesystem::path bad = path;
            bad += ".bad";
            std::filesystem::path tmp = path;
            tmp += ".tmp";
            std::filesystem::remove(path);
            std::filesystem::remove(bad);

            // An object written over an object merges into it: what another
            // writer keeps under the key stays, what the object brings
            // wins.
            {
                auto settings = Settings::create(_context, path);
                settings->set("/Group/Recent", nlohmann::json::array({ "a", "b" }));
                settings->set("/Group", nlohmann::json({ { "Author", "me" }, { "Recent", nlohmann::json::array({ "c" }) } }));
                settings->set("/Group", nlohmann::json({ { "Author", "you" } }));
                nlohmann::json json;
                FTK_CHECK(settings->get("/Group", json));
                FTK_CHECK("you" == json.value("Author", ""));
                FTK_CHECK(json.contains("Recent") && 1 == json.at("Recent").size());
                // Anything else replaces.
                settings->set("/Group", 3);
                int i = 0;
                FTK_CHECK(settings->get("/Group", i) && 3 == i);
            }

            // A value round-trips through the file, and the file it is
            // written through is gone.
            {
                auto settings = Settings::create(_context, path);
                settings->set("/A", true);
                settings->save();
                FTK_CHECK(std::filesystem::exists(path));
                FTK_CHECK(!std::filesystem::exists(tmp));
            }
            {
                auto settings = Settings::create(_context, path);
                bool a = false;
                FTK_CHECK(settings->get("/A", a) && a);
            }

            // A file that cannot be read -- here one cut off half way, the
            // way a second instance found it while the first was writing
            // -- is set aside rather than overwritten with the defaults.
            {
                std::string contents = read(FileIO::create(path, FileMode::Read));
                contents.resize(contents.size() / 2);
                FileIO::create(path, FileMode::Write)->write(contents);
            }
            {
                auto settings = Settings::create(_context, path);
                bool a = false;
                FTK_CHECK(!settings->get("/A", a));
                FTK_CHECK(std::filesystem::exists(bad));
                FTK_CHECK(!std::filesystem::exists(path));
            }
            FTK_CHECK(std::filesystem::exists(path));
            {
                auto settings = Settings::create(_context, path);
                bool a = false;
                FTK_CHECK(!settings->get("/A", a));
            }
            std::filesystem::remove(path);
            std::filesystem::remove(bad);
        }
    }
}
