// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class OSTest : public test::ITest
        {
        protected:
            OSTest(const std::shared_ptr<Context>&);

        public:
            virtual ~OSTest();

            static std::shared_ptr<OSTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _env();
        };
    }
}

