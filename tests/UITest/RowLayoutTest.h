// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/App.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class RowLayoutTest : public test::ITest
        {
        protected:
            RowLayoutTest(const std::shared_ptr<Context>&);

        public:
            virtual ~RowLayoutTest();

            static std::shared_ptr<RowLayoutTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _test(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<App>&,
                const std::shared_ptr<Window>&,
                const std::shared_ptr<RowLayout>&,
                Orientation);
        };
    }
}

