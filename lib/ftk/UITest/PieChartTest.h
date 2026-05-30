// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class PieChartTest : public test::ITest
        {
        protected:
            PieChartTest(const std::shared_ptr<Context>&);

        public:
            virtual ~PieChartTest();

            static std::shared_ptr<PieChartTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

