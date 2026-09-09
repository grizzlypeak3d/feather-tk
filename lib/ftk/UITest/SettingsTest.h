// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class SettingsTest : public test::ITest
        {
        protected:
            SettingsTest(const std::shared_ptr<Context>&);

        public:
            virtual ~SettingsTest();

            static std::shared_ptr<SettingsTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}
