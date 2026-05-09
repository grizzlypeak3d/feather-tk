// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace gl_test
    {
        class UtilTest : public test::ITest
        {
        protected:
            UtilTest(const std::shared_ptr<Context>&);

        public:
            virtual ~UtilTest();

            static std::shared_ptr<UtilTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

