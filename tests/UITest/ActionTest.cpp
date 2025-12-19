// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ActionTest.h>

#include <ftk/UI/Action.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ActionTest::ActionTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ActionTest")
        {}

        ActionTest::~ActionTest()
        {}

        std::shared_ptr<ActionTest> ActionTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ActionTest>(new ActionTest(context));
        }
                
        void ActionTest::run()
        {
            {
                auto a = Action::create(
                    "Test",
                    [] {});
                FTK_ASSERT("Test" == a->getText());
            }
            {
                auto a = Action::create(
                    "Test",
                    "Icon",
                    [] {});
                FTK_ASSERT("Test" == a->getText());
                FTK_ASSERT("Icon" == a->getIcon());
            }
            {
                const KeyShortcut shortcut(Key::T, static_cast<int>(KeyModifier::Control));
                auto a = Action::create(
                    "Test",
                    shortcut,
                    [] {});
                FTK_ASSERT("Test" == a->getText());
                FTK_ASSERT(shortcut == a->getShortcuts().front());
            }
            {
                const KeyShortcut shortcut(Key::T, static_cast<int>(KeyModifier::Control));
                auto a = Action::create(
                    "Test",
                    "Icon",
                    shortcut,
                    [] {});
                FTK_ASSERT("Test" == a->getText());
                FTK_ASSERT("Icon" == a->getIcon());
                FTK_ASSERT(shortcut == a->getShortcuts().front());
            }
            {
                auto a = Action::create(
                    "Test",
                    [](bool) {});
                FTK_ASSERT("Test" == a->getText());
            }
            {
                auto a = Action::create(
                    "Test",
                    "Icon",
                    [](bool) {});
                FTK_ASSERT("Test" == a->getText());
                FTK_ASSERT("Icon" == a->getIcon());
            }
            {
                const KeyShortcut shortcut(Key::T, static_cast<int>(KeyModifier::Control));
                auto a = Action::create(
                    "Test",
                    shortcut,
                    [](bool) {});
                FTK_ASSERT("Test" == a->getText());
                FTK_ASSERT(shortcut == a->getShortcuts().front());
            }
            {
                const KeyShortcut shortcut(Key::T, static_cast<int>(KeyModifier::Control));
                auto a = Action::create(
                    "Test",
                    "Icon",
                    shortcut,
                    [](bool) {});
                FTK_ASSERT("Test" == a->getText());
                FTK_ASSERT("Icon" == a->getIcon());
                FTK_ASSERT(shortcut == a->getShortcuts().front());
            }
        }
    }
}

