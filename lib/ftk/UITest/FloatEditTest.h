// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FloatEditTest : public test::ITest
        {
        protected:
            FloatEditTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FloatEditTest();

            static std::shared_ptr<FloatEditTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

