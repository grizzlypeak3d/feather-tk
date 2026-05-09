// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class WidgetOptionsTest : public test::ITest
        {
        protected:
            WidgetOptionsTest(const std::shared_ptr<Context>&);

        public:
            virtual ~WidgetOptionsTest();

            static std::shared_ptr<WidgetOptionsTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
        };
    }
}

