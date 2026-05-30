// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/App.h>
#include <ftk/UI/ButtonGroup.h>
#include <ftk/UI/Window.h>

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ListWidgetTest : public test::ITest
        {
        protected:
            ListWidgetTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ListWidgetTest();

            static std::shared_ptr<ListWidgetTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _test(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<App>&,
                const std::shared_ptr<Window>&,
                ButtonGroupType);
        };
    }
}

