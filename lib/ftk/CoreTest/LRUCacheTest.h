// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class LRUCacheTest : public test::ITest
        {
        protected:
            LRUCacheTest(const std::shared_ptr<Context>&);

        public:
            virtual ~LRUCacheTest();

            static std::shared_ptr<LRUCacheTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

