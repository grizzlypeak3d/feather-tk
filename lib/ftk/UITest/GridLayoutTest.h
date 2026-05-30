// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class GridLayoutTest : public test::ITest
        {
        protected:
            GridLayoutTest(const std::shared_ptr<Context>&);

        public:
            virtual ~GridLayoutTest();

            static std::shared_ptr<GridLayoutTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

