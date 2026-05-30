// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ProgressDialogTest : public test::ITest
        {
        protected:
            ProgressDialogTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ProgressDialogTest();

            static std::shared_ptr<ProgressDialogTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

