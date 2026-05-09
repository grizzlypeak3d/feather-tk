// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ButtonGroupTest : public test::ITest
        {
        protected:
            ButtonGroupTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ButtonGroupTest();

            static std::shared_ptr<ButtonGroupTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

