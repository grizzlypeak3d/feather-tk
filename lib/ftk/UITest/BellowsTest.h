// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class BellowsTest : public test::ITest
        {
        protected:
            BellowsTest(const std::shared_ptr<Context>&);

        public:
            virtual ~BellowsTest();

            static std::shared_ptr<BellowsTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

