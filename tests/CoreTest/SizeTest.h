// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class SizeTest : public test::ITest
        {
        protected:
            SizeTest(const std::shared_ptr<Context>&);

        public:
            virtual ~SizeTest();

            static std::shared_ptr<SizeTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _members();
            void _functions();
            void _operators();
            void _serialize();
        };
    }
}

