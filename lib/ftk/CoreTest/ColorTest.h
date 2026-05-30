// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class ColorTest : public test::ITest
        {
        protected:
            ColorTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ColorTest();

            static std::shared_ptr<ColorTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _members();
            void _operators();
            void _functions();
            void _serialize();
        };
    }
}

