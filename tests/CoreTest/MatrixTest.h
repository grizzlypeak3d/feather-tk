// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class MatrixTest : public test::ITest
        {
        protected:
            MatrixTest(const std::shared_ptr<Context>&);

        public:
            virtual ~MatrixTest();

            static std::shared_ptr<MatrixTest> create(
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

