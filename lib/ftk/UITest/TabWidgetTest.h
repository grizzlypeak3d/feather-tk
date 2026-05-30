// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class TabWidgetTest : public test::ITest
        {
        protected:
            TabWidgetTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TabWidgetTest();

            static std::shared_ptr<TabWidgetTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _bar();
            void _widget();
        };
    }
}

