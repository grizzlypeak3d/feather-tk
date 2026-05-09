// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ActionTest : public test::ITest
        {
        protected:
            ActionTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ActionTest();

            static std::shared_ptr<ActionTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

