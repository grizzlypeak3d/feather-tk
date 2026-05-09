// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class TextEditModelTest : public test::ITest
        {
        protected:
            TextEditModelTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TextEditModelTest();

            static std::shared_ptr<TextEditModelTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

