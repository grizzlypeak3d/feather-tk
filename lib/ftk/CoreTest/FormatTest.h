// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class FormatTest : public test::ITest
        {
        protected:
            FormatTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FormatTest();

            static std::shared_ptr<FormatTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

