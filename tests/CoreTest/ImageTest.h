// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class ImageTest : public test::ITest
        {
        protected:
            ImageTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ImageTest();

            static std::shared_ptr<ImageTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
            
        private:
            void _enums();
            void _info();
            void _members();
            void _functions();
        };
    }
}

