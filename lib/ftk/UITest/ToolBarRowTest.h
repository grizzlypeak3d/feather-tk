// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ToolBarRowTest : public test::ITest
        {
        protected:
            ToolBarRowTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ToolBarRowTest();

            static std::shared_ptr<ToolBarRowTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}
