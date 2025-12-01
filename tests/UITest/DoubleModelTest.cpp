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
            if (auto context = _context.lock())
            {
                auto model = DoubleModel::create(context);

                double value = 0.0;
                RangeD range;
                bool hasDefaultValue = false;
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
                    model->observeHasDefaultValue(),
                    [&hasDefaultValue](bool value)
                    {
                        hasDefaultValue = value;
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
                model->decrementStep();
                FTK_ASSERT(fuzzyCompare(0.3, value));
                model->incrementStep();
                FTK_ASSERT(fuzzyCompare(0.5, value));

                model->setLargeStep(0.3);
                FTK_ASSERT(0.3 == model->getLargeStep());
                model->decrementLargeStep();
                FTK_ASSERT(fuzzyCompare(0.2, value));
                model->incrementLargeStep();
                FTK_ASSERT(fuzzyCompare(0.5, value));

                model->setDefaultValue(0.0);
                FTK_ASSERT(0.0 == model->getDefaultValue());
                FTK_ASSERT(hasDefaultValue);
                model->setDefaultValue();
                FTK_ASSERT(0.0 == value);
                model->clearDefaultValue();
                FTK_ASSERT(!hasDefaultValue);
            }
        }
    }
}

