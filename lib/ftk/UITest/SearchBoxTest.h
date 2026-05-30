// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class SearchBoxTest : public test::ITest
        {
        protected:
            SearchBoxTest(const std::shared_ptr<Context>&);

        public:
            virtual ~SearchBoxTest();

            static std::shared_ptr<SearchBoxTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

