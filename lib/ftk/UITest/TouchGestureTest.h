// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class TouchGestureTest : public test::ITest
        {
        protected:
            TouchGestureTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TouchGestureTest();

            static std::shared_ptr<TouchGestureTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _pan();
            void _pinch();
            void _undecided();
            void _fingers();
            void _mouseDelay();
        };
    }
}
