// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/DoubleModelTest.h>

#include <ftk/UI/DoubleModel.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Math.h>

namespace ftk
{
    namespace ui_test
    {
        DoubleModelTest::DoubleModelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::DoubleModelTest")
        {}

        DoubleModelTest::~DoubleModelTest()
        {}

        std::shared_ptr<DoubleModelTest> DoubleModelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DoubleModelTest>(new DoubleModelTest(context));
        }
                
        void DoubleModelTest::run()
        {
            auto model = DoubleModel::create();

            double value = 0.0;
            RangeD range;
            bool hasDefault = false;
            auto valueObserver = Observer<double>::create(
                model->observeValue(),
                [&value](double v)
                {
                    value = v;
                });
            auto rangeObserver = Observer<RangeD>::create(
                model->observeRange(),
                [&range](const RangeD& r)
                {
                    range = r;
                });
            auto defaultObserver = Observer<bool>::create(
                model->observeHasDefault(),
                [&hasDefault](bool value)
                {
                    hasDefault = value;
                });

            model->setValue(0.9);
            FTK_ASSERT(0.9 == model->getValue());
            FTK_ASSERT(0.9 == value);

            model->setRange(RangeD(0.0, 0.5));
            FTK_ASSERT(RangeD(0.0, 0.5) == model->getRange());
            FTK_ASSERT(RangeD(0.0, 0.5) == range);
            FTK_ASSERT(0.5 == value);

            model->setStep(0.2);
            FTK_ASSERT(0.2 == model->getStep());
            model->stepDec();
            FTK_ASSERT(fuzzyCompare(0.3, value));
            model->step();
            FTK_ASSERT(fuzzyCompare(0.5, value));

            model->setLargeStep(0.3);
            FTK_ASSERT(0.3 == model->getLargeStep());
            model->largeStepDec();
            FTK_ASSERT(fuzzyCompare(0.2, value));
            model->largeStep();
            FTK_ASSERT(fuzzyCompare(0.5, value));

            model->setDefault(0.0);
            FTK_ASSERT(0.0 == model->getDefault());
            FTK_ASSERT(hasDefault);
            model->setDefault();
            FTK_ASSERT(0.0 == value);
            model->clearDefault();
            FTK_ASSERT(!hasDefault);
        }
    }
}

