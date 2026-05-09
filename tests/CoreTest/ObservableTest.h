// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class ObservableTest : public test::ITest
        {
        protected:
            ObservableTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ObservableTest();

            static std::shared_ptr<ObservableTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _value();
            void _list();
            void _map();
        };
    }
}

