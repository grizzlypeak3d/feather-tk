// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class DrawUtilTest : public test::ITest
        {
        protected:
            DrawUtilTest(const std::shared_ptr<Context>&);

        public:
            virtual ~DrawUtilTest();

            static std::shared_ptr<DrawUtilTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

