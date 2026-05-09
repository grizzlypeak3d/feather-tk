// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace gl_test
    {
        class WindowTest : public test::ITest
        {
        protected:
            WindowTest(const std::shared_ptr<Context>&);

        public:
            virtual ~WindowTest();

            static std::shared_ptr<WindowTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
            void _members();
            void _functions();
        };
    }
}

