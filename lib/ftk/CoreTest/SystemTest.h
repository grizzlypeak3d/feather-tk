// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class SystemTest : public test::ITest
        {
        protected:
            SystemTest(const std::shared_ptr<Context>&);

        public:
            virtual ~SystemTest();

            static std::shared_ptr<SystemTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

