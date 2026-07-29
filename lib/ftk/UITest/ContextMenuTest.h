// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ContextMenuTest : public test::ITest
        {
        protected:
            ContextMenuTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ContextMenuTest();

            static std::shared_ptr<ContextMenuTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}
