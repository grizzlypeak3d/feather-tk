// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class PNGTest : public test::ITest
        {
        protected:
            PNGTest(const std::shared_ptr<Context>&);

        public:
            virtual ~PNGTest();

            static std::shared_ptr<PNGTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

