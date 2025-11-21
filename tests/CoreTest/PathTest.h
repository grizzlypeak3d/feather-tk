// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class PathTest : public test::ITest
        {
        protected:
            PathTest(const std::shared_ptr<Context>&);

        public:
            virtual ~PathTest();

            static std::shared_ptr<PathTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _enums();
            void _util();
            void _split();
            void _drives();
            void _userPaths();
            void _path();
            void _dirList();
            void _expandSeq();
        };
    }
}

