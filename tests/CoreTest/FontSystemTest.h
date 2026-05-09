// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class FontSystemTest : public test::ITest
        {
        protected:
            FontSystemTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FontSystemTest();

            static std::shared_ptr<FontSystemTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _info();
            void _size();
            void _add();
        };
    }
}

