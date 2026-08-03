// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/IntModelTest.h>

#include <ftk/UI/IntModel.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        IntModelTest::IntModelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::IntModelTest")
        {}

        IntModelTest::~IntModelTest()
        {}

        std::shared_ptr<IntModelTest> IntModelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<IntModelTest>(new IntModelTest(context));
        }
                
        void IntModelTest::run()
        {
            auto model = IntModel::create();

            int value = 0;
            RangeI range;
            bool hasDefault = false;
            auto valueObserver = Observer<int>::create(
                model->observeValue(),
                [&value](int v)
                {
                    value = v;
                });
            auto rangeObserver = Observer<RangeI>::create(
                model->observeRange(),
                [&range](const RangeI& r)
                {
                    range = r;
                });
            auto defaultObserver = Observer<bool>::create(
                model->observeHasDefault(),
                [&hasDefault](bool value)
                {
                    hasDefault = value;
                });

            model->setValue(11);
            FTK_CHECK(11 == model->getValue());
            FTK_CHECK(11 == value);

            model->setRange(RangeI(0, 10));
            FTK_CHECK(RangeI(0, 10) == model->getRange());
            FTK_CHECK(RangeI(0, 10) == range);
            FTK_CHECK(10 == value);

            model->setStep(2);
            FTK_CHECK(2 == model->getStep());
            model->stepDec();
            FTK_CHECK(8 == value);
            model->step();
            FTK_CHECK(10 == value);

            model->setLargeStep(5);
            FTK_CHECK(5 == model->getLargeStep());
            model->largeStepDec();
            FTK_CHECK(5 == value);
            model->largeStep();
            FTK_CHECK(10 == value);

            model->setDefault(0);
            FTK_CHECK(0 == model->getDefault());
            FTK_CHECK(hasDefault);
            model->setDefault();
            FTK_CHECK(0 == value);
            model->clearDefault();
            FTK_CHECK(!hasDefault);
        }
    }
}

