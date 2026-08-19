// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/RecentFilesModelTest.h>

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
            {
                auto model = RecentFilesModel::create(_context);

                size_t recentMax = 0;
                std::vector<Path> recent;
                auto recentMaxObserver = Observer<size_t>::create(
                    model->observeRecentMax(),
                    [&recentMax](size_t value)
                    {
                        recentMax = value;
                    });
                auto recentObserver = ListObserver<Path>::create(
                    model->observeRecent(),
                    [&recent](const std::vector<Path>& value)
                    {
                        recent = value;
                    });

                model->setRecentMax(2);
                model->setRecentMax(2);
                FTK_CHECK(2 == model->getRecentMax());
                FTK_CHECK(2 == recentMax);

                std::vector<Path> recent2;
                recent2.push_back(Path("path 0"));
                recent2.push_back(Path("path 1"));
                recent2.push_back(Path("path 2"));
                model->setRecent(recent2);
                recent2.clear();
                recent2.push_back(Path("path 1"));
                recent2.push_back(Path("path 2"));
                FTK_CHECK(recent2[0].get() == model->getRecent()[0].getFileName());
                FTK_CHECK(recent2[1].get() == model->getRecent()[1].getFileName());
                model->addRecent(Path("path 3"));
                recent2.clear();
                recent2.push_back(Path("path 2"));
                recent2.push_back(Path("path 3"));
                FTK_CHECK(recent2[0].get() == recent[0].getFileName());
                FTK_CHECK(recent2[1].get() == recent[1].getFileName());

                model->setRecentMax(1);
                recent2.clear();
                recent2.push_back(Path("path 3"));
                FTK_CHECK(recent2[0].get() == recent[0].getFileName());
                model->addRecent(Path("path 3"));
                FTK_CHECK(recent2[0].get() == recent[0].getFileName());
            }
            {
                // A recent file keeps the range it was opened over. That is
                // what says whether it was one frame or the sequence it sits
                // in, so the two are different entries and opening one again
                // opens what it opened before.
                auto model = RecentFilesModel::create(_context);
                Path seq("render.0001.exr");
                seq.setFrames(RangeI64(1, 100));
                model->addRecent(seq);
                model->addRecent(Path("render.0001.exr"));
                FTK_CHECK(2 == model->getRecent().size());
                FTK_CHECK(model->getRecent()[0].isSeq());
                FTK_CHECK(RangeI64(1, 100) == model->getRecent()[0].getFrames().value());
                // Not a range of nothing: parsing a number out of a file name
                // gives a range of that one frame, which is what a lone frame
                // is and is why the two entries are not the same.
                FTK_CHECK(!model->getRecent()[1].isSeq());
            }
        }
    }
}

