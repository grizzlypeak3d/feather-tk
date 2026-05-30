// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FloatSliderTest : public test::ITest
        {
        protected:
            FloatSliderTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FloatSliderTest();

            static std::shared_ptr<FloatSliderTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

