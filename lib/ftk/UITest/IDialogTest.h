// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class IDialogTest : public test::ITest
        {
        protected:
            IDialogTest(const std::shared_ptr<Context>&);

        public:
            virtual ~IDialogTest();

            static std::shared_ptr<IDialogTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}
