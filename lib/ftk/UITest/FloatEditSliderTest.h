// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FloatEditSliderTest : public test::ITest
        {
        protected:
            FloatEditSliderTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FloatEditSliderTest();

            static std::shared_ptr<FloatEditSliderTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

