// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class LineEditModelTest : public test::ITest
        {
        protected:
            LineEditModelTest(const std::shared_ptr<Context>&);

        public:
            virtual ~LineEditModelTest();

            static std::shared_ptr<LineEditModelTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

