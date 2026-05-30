// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class MemoryTest : public test::ITest
        {
        protected:
            MemoryTest(const std::shared_ptr<Context>&);

        public:
            virtual ~MemoryTest();

            static std::shared_ptr<MemoryTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
            void _endian();
            void _bits();
        };
    }
}

