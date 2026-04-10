// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/LineEditModelTest.h>

#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/LineEditModel.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        LineEditModelTest::LineEditModelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::LineEditModelTest")
        {}

        LineEditModelTest::~LineEditModelTest()
        {}

        std::shared_ptr<LineEditModelTest> LineEditModelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<LineEditModelTest>(new LineEditModelTest(context));
        }
                
        void LineEditModelTest::run()
        {
            if (auto context = _context.lock())
            {
                auto model = LineEditModel::create(context);
                std::string text = "abcdefghijklmnopqrstuvwxyz";
                model->setText(text);
                model->setText(text);
                FTK_ASSERT(text == model->getText());

                std::string text2;
                auto observer = Observer<std::string>::create(
                    model->observeText(),
                    [&text2](const std::string& value)
                    {
                        text2 = value;
                    });
                FTK_ASSERT(text2 == text);
                model->clearText();
                FTK_ASSERT(text2 == std::string());
            }
            if (auto context = _context.lock())
            {
                auto model = LineEditModel::create(context);
                FTK_ASSERT(model->key(Key::A));
                FTK_ASSERT(model->key(Key::A, static_cast<int>(commandKeyModifier)));
            }
        }
    }
}

