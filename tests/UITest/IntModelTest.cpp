// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/IntModelTest.h>

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
            FTK_ASSERT(11 == model->getValue());
            FTK_ASSERT(11 == value);

            model->setRange(RangeI(0, 10));
            FTK_ASSERT(RangeI(0, 10) == model->getRange());
            FTK_ASSERT(RangeI(0, 10) == range);
            FTK_ASSERT(10 == value);

            model->setStep(2);
            FTK_ASSERT(2 == model->getStep());
            model->stepDec();
            FTK_ASSERT(8 == value);
            model->step();
            FTK_ASSERT(10 == value);

            model->setLargeStep(5);
            FTK_ASSERT(5 == model->getLargeStep());
            model->largeStepDec();
            FTK_ASSERT(5 == value);
            model->largeStep();
            FTK_ASSERT(10 == value);

            model->setDefault(0);
            FTK_ASSERT(0 == model->getDefault());
            FTK_ASSERT(hasDefault);
            model->setDefault();
            FTK_ASSERT(0 == value);
            model->clearDefault();
            FTK_ASSERT(!hasDefault);
        }
    }
}

