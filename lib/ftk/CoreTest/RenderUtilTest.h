// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class RenderUtilTest : public test::ITest
        {
        protected:
            RenderUtilTest(const std::shared_ptr<Context>&);

        public:
            virtual ~RenderUtilTest();

            static std::shared_ptr<RenderUtilTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

