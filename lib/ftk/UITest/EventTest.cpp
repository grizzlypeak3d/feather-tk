// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/EventTest.h>

#include <ftk/UI/Event.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        EventTest::EventTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::EventTest")
        {}

        EventTest::~EventTest()
        {}

        std::shared_ptr<EventTest> EventTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<EventTest>(new EventTest(context));
        }
                
        void EventTest::run()
        {
            _enums();
        }
        
        void EventTest::_enums()
        {
            FTK_TEST_ENUM(Key);
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::None)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Shift)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Control)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Alt)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Super)));
            _print(getShortcutLabel(Key::O, static_cast<int>(KeyModifier::Control)));
        }
    }
}

