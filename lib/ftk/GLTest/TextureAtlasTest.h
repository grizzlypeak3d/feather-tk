// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace gl_test
    {
        class TextureAtlasTest : public test::ITest
        {
        protected:
            TextureAtlasTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TextureAtlasTest();

            static std::shared_ptr<TextureAtlasTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

