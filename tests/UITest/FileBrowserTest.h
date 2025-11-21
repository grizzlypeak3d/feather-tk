// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FileBrowserTest : public test::ITest
        {
        protected:
            FileBrowserTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FileBrowserTest();

            static std::shared_ptr<FileBrowserTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _shortcuts();
            void _view();
            void _widget();
            void _dialog();
        };
    }
}

