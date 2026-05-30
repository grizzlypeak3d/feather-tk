// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class MDIWidgetTest : public test::ITest
        {
        protected:
            MDIWidgetTest(const std::shared_ptr<Context>&);

        public:
            virtual ~MDIWidgetTest();

            static std::shared_ptr<MDIWidgetTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
            void _widget();
        };
    }
}

