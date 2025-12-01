// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/RecentFilesModelTest.h>

#include <ftk/UI/RecentFilesModel.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        RecentFilesModelTest::RecentFilesModelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::RecentFilesModelTest")
        {}

        RecentFilesModelTest::~RecentFilesModelTest()
        {}

        std::shared_ptr<RecentFilesModelTest> RecentFilesModelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<RecentFilesModelTest>(new RecentFilesModelTest(context));
        }
                
        void RecentFilesModelTest::run()
        {
            if (auto context = _context.lock())
            {
                auto model = RecentFilesModel::create(context);

                size_t recentMax = 0;
                std::vector<std::filesystem::path> recent;
                auto recentMaxObserver = Observer<size_t>::create(
                    model->observeRecentMax(),
                    [&recentMax](size_t value)
                    {
                        recentMax = value;
                    });
                auto recentObserver = ListObserver<std::filesystem::path>::create(
                    model->observeRecent(),
                    [&recent](const std::vector<std::filesystem::path>& value)
                    {
                        recent = value;
                    });

                model->setRecentMax(2);
                model->setRecentMax(2);
                FTK_ASSERT(2 == model->getRecentMax());
                FTK_ASSERT(2 == recentMax);

                std::vector<std::filesystem::path> recent2;
                recent2.push_back("path 0");
                recent2.push_back("path 1");
                recent2.push_back("path 2");
                model->setRecent(recent2);
                recent2.clear();
                recent2.push_back("path 1");
                recent2.push_back("path 2");
                FTK_ASSERT(recent2 == model->getRecent());
                FTK_ASSERT(recent2 == recent);
                model->addRecent("path 3");
                recent2.clear();
                recent2.push_back("path 2");
                recent2.push_back("path 3");
                FTK_ASSERT(recent2 == recent);

                model->setRecentMax(1);
                recent2.clear();
                recent2.push_back("path 3");
                FTK_ASSERT(recent2 == recent);
                model->addRecent("path 3");
                FTK_ASSERT(recent2 == recent);
            }
        }
    }
}

