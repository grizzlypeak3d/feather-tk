// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ScrollAreaTest : public test::ITest
        {
        protected:
            ScrollAreaTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ScrollAreaTest();

            static std::shared_ptr<ScrollAreaTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
        };
    }
}

