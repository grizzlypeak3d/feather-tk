// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class LayoutUtilTest : public test::ITest
        {
        protected:
            LayoutUtilTest(const std::shared_ptr<Context>&);

        public:
            virtual ~LayoutUtilTest();

            static std::shared_ptr<LayoutUtilTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

