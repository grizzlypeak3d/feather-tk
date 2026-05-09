// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class MessageDialogTest : public test::ITest
        {
        protected:
            MessageDialogTest(const std::shared_ptr<Context>&);

        public:
            virtual ~MessageDialogTest();

            static std::shared_ptr<MessageDialogTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

