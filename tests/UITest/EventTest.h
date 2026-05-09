// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class EventTest : public test::ITest
        {
        protected:
            EventTest(const std::shared_ptr<Context>&);

        public:
            virtual ~EventTest();

            static std::shared_ptr<EventTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
        };
    }
}

