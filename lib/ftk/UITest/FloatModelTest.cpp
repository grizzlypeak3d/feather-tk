// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FloatModelTest.h>

#include <ftk/UI/FloatModel.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Math.h>

namespace ftk
{
    namespace ui_test
    {
        FloatModelTest::FloatModelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::FloatModelTest")
        {}

        FloatModelTest::~FloatModelTest()
        {}

        std::shared_ptr<FloatModelTest> FloatModelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FloatModelTest>(new FloatModelTest(context));
        }
                
        void FloatModelTest::run()
        {
            auto model = FloatModel::create();

            float value = 0.F;
            RangeF range;
            bool hasDefault = false;
            auto valueObserver = Observer<float>::create(
                model->observeValue(),
                [&value](float v)
                {
                    value = v;
                });
            auto rangeObserver = Observer<RangeF>::create(
                model->observeRange(),
                [&range](const RangeF& r)
                {
                    range = r;
                });
            auto defaultObserver = Observer<bool>::create(
                model->observeHasDefault(),
                [&hasDefault](bool value)
                {
                    hasDefault = value;
                });

            model->setValue(.9F);
            FTK_CHECK(.9F == model->getValue());
            FTK_CHECK(.9F == value);

            model->setRange(RangeF(0.F, .5F));
            FTK_CHECK(RangeF(0.F, .5F) == model->getRange());
            FTK_CHECK(RangeF(0.F, .5F) == range);
            FTK_CHECK(.5F == value);

            model->setStep(.2F);
            FTK_CHECK(.2F == model->getStep());
            model->stepDec();
            FTK_CHECK(fuzzyCompare(.3F, value));
            model->step();
            FTK_CHECK(fuzzyCompare(.5F, value));

            model->setLargeStep(.3F);
            FTK_CHECK(.3F == model->getLargeStep());
            model->largeStepDec();
            FTK_CHECK(fuzzyCompare(.2F, value));
            model->largeStep();
            FTK_CHECK(fuzzyCompare(.5F, value));

            model->setDefault(0.F);
            FTK_CHECK(0.F == model->getDefault());
            FTK_CHECK(hasDefault);
            model->setDefault();
            FTK_CHECK(0.F == value);
            model->clearDefault();
            FTK_CHECK(!hasDefault);

            // A soft range extends to admit an out of range value instead
            // of clamping it.
            model->setRange(RangeF(0.F, .5F));
            model->setRangeSoft(true);
            FTK_CHECK(model->isRangeSoft());
            model->setValue(2.F);
            FTK_CHECK(2.F == value);
            FTK_CHECK(RangeF(0.F, 2.F) == range);
            model->setValue(-1.F);
            FTK_CHECK(-1.F == value);
            FTK_CHECK(RangeF(-1.F, 2.F) == range);
            model->setRangeSoft(false);
            model->setValue(3.F);
            FTK_CHECK(2.F == value);
        }
    }
}
