// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class StyleTest : public test::ITest
        {
        protected:
            StyleTest(const std::shared_ptr<Context>&);

        public:
            virtual ~StyleTest();

            static std::shared_ptr<StyleTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
            void _widget();
        };
    }
}

