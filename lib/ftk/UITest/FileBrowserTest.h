// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

#include <ftk/UI/IWidget.h>

namespace ftk
{
    namespace ui_test
    {
        class FileBrowserTest : public test::ITest
        {
        protected:
            FileBrowserTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FileBrowserTest();

            static std::shared_ptr<FileBrowserTest> create(
                const std::shared_ptr<Context>&);

            void run() override;

        private:
            void _shortcuts();
            void _view();
            void _selection();
            void _click(const std::shared_ptr<IWidget>&, const std::string& text);

            //! Find the first widget of the given type below this one.
            template<typename T>
            static std::shared_ptr<T> _find(const std::shared_ptr<IWidget>& widget)
            {
                if (auto out = std::dynamic_pointer_cast<T>(widget))
                {
                    return out;
                }
                for (const auto& child : widget->getChildren())
                {
                    if (auto out = _find<T>(child))
                    {
                        return out;
                    }
                }
                return nullptr;
            }
            void _widget();
            void _dialog();
        };
    }
}

