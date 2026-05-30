// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FileEditTest : public test::ITest
        {
        protected:
            FileEditTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FileEditTest();

            static std::shared_ptr<FileEditTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

