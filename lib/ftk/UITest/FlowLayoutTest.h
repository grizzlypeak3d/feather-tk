// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FlowLayoutTest : public test::ITest
        {
        protected:
            FlowLayoutTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FlowLayoutTest();

            static std::shared_ptr<FlowLayoutTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}
