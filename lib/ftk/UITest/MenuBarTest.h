// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class MenuBarTest : public test::ITest
        {
        protected:
            MenuBarTest(const std::shared_ptr<Context>&);

        public:
            virtual ~MenuBarTest();

            static std::shared_ptr<MenuBarTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

