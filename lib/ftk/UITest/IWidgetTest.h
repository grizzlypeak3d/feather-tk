// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class IWidgetTest : public test::ITest
        {
        protected:
            IWidgetTest(const std::shared_ptr<Context>&);

        public:
            virtual ~IWidgetTest();

            static std::shared_ptr<IWidgetTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

