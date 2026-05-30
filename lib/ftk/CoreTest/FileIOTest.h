// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class FileIOTest : public test::ITest
        {
        protected:
            FileIOTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FileIOTest();

            static std::shared_ptr<FileIOTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
            void _members();
            void _functions();
            void _operators();
        };
    }
}

