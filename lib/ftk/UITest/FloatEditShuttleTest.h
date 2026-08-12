// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FloatEditShuttleTest : public test::ITest
        {
        protected:
            FloatEditShuttleTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FloatEditShuttleTest();

            static std::shared_ptr<FloatEditShuttleTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}
