// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class LineEditTest : public test::ITest
        {
        protected:
            LineEditTest(const std::shared_ptr<Context>&);

        public:
            virtual ~LineEditTest();

            static std::shared_ptr<LineEditTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

