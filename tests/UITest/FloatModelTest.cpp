// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/FloatModelTest.h>

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
            if (auto context = _context.lock())
            {
                auto model = FloatModel::create(context);

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
                FTK_ASSERT(.9F == model->getValue());
                FTK_ASSERT(.9F == value);

                model->setRange(RangeF(0.F, .5F));
                FTK_ASSERT(RangeF(0.F, .5F) == model->getRange());
                FTK_ASSERT(RangeF(0.F, .5F) == range);
                FTK_ASSERT(.5F == value);

                model->setStep(.2F);
                FTK_ASSERT(.2F == model->getStep());
                model->stepDec();
                FTK_ASSERT(fuzzyCompare(.3F, value));
                model->step();
                FTK_ASSERT(fuzzyCompare(.5F, value));

                model->setLargeStep(.3F);
                FTK_ASSERT(.3F == model->getLargeStep());
                model->largeStepDec();
                FTK_ASSERT(fuzzyCompare(.2F, value));
                model->largeStep();
                FTK_ASSERT(fuzzyCompare(.5F, value));

                model->setDefault(0.F);
                FTK_ASSERT(0.F == model->getDefault());
                FTK_ASSERT(hasDefault);
                model->setDefault();
                FTK_ASSERT(0.F == value);
                model->clearDefault();
                FTK_ASSERT(!hasDefault);
            }
        }
    }
}
