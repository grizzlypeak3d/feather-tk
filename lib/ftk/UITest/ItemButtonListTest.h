// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class ItemButtonListTest : public test::ITest
        {
        protected:
            ItemButtonListTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ItemButtonListTest();

            static std::shared_ptr<ItemButtonListTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _doubleClick();
        };
    }
}
