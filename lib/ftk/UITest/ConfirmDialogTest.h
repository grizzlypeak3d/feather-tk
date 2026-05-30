// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ConfirmDialogTest : public test::ITest
        {
        protected:
            ConfirmDialogTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ConfirmDialogTest();

            static std::shared_ptr<ConfirmDialogTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

