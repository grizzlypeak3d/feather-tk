// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class DragDropTest : public test::ITest
        {
        protected:
            DragDropTest(const std::shared_ptr<Context>&);

        public:
            virtual ~DragDropTest();

            static std::shared_ptr<DragDropTest> create(const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

