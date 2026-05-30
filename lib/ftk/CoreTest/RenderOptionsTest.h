// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class RenderOptionsTest : public test::ITest
        {
        protected:
            RenderOptionsTest(const std::shared_ptr<Context>&);

        public:
            virtual ~RenderOptionsTest();

            static std::shared_ptr<RenderOptionsTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
            void _operators();
        };
    }
}

