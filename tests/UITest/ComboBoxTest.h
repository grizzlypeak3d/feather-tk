// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ComboBoxTest : public test::ITest
        {
        protected:
            ComboBoxTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ComboBoxTest();

            static std::shared_ptr<ComboBoxTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

