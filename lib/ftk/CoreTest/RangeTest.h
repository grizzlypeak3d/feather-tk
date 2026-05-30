// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class RangeTest : public test::ITest
        {
        protected:
            RangeTest(const std::shared_ptr<Context>&);

        public:
            virtual ~RangeTest();

            static std::shared_ptr<RangeTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _members();
            void _functions();
            void _serialize();
        };
    }
}

