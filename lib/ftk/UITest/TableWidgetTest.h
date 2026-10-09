// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class TableWidgetTest : public test::ITest
        {
        protected:
            TableWidgetTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TableWidgetTest();

            static std::shared_ptr<TableWidgetTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

