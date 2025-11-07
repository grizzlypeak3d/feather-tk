// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <TestLib/ITest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IButton.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    namespace ui_test
    {
        class ButtonTest : public test::ITest
        {
        protected:
            ButtonTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ButtonTest();

            static std::shared_ptr<ButtonTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _test(
                const std::shared_ptr<App>&,
                const std::shared_ptr<IWindow>&,
                const std::shared_ptr<VerticalLayout>&,
                const std::shared_ptr<IButton>&);
        };
    }
}

