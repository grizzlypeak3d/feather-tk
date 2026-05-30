// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class GroupBoxTest : public test::ITest
        {
        protected:
            GroupBoxTest(const std::shared_ptr<Context>&);

        public:
            virtual ~GroupBoxTest();

            static std::shared_ptr<GroupBoxTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

