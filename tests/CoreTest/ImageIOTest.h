// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class ImageIOTest : public test::ITest
        {
        protected:
            ImageIOTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ImageIOTest();

            static std::shared_ptr<ImageIOTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
            
        private:
            void _members();
            void _functions();
        };
    }
}

