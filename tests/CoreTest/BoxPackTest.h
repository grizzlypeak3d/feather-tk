// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

#include <ftk/Core/BoxPack.h>

namespace ftk
{
    namespace core_test
    {
        class BoxPackTest : public test::ITest
        {
        protected:
            BoxPackTest(const std::shared_ptr<Context>&);

        public:
            virtual ~BoxPackTest();

            static std::shared_ptr<BoxPackTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _printPack(const std::shared_ptr<BoxPack>&);
            void _printNode(
                const std::shared_ptr<BoxPackNode>&,
                int indent = 0,
                char pad = ' ');
        };
    }
}

