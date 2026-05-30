// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class BoxTest : public test::ITest
        {
        protected:
            BoxTest(const std::shared_ptr<Context>&);

        public:
            virtual ~BoxTest();

            static std::shared_ptr<BoxTest> create(
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

