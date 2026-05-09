// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class LabelTest : public test::ITest
        {
        protected:
            LabelTest(const std::shared_ptr<Context>&);

        public:
            virtual ~LabelTest();

            static std::shared_ptr<LabelTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

