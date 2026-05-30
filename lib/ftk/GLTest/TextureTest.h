// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace gl_test
    {
        class TextureTest : public test::ITest
        {
        protected:
            TextureTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TextureTest();

            static std::shared_ptr<TextureTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _members();
            void _functions();
            void _operators();
        };
    }
}

