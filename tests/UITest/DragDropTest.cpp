// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/DragDropTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Spacer.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        namespace
        {
            class DragDropWidget : public IMouseWidget
            {
            protected:
                void _init(
                    const std::shared_ptr<Context>&,
                    const std::vector<std::string>& text,
                    const std::shared_ptr<IWidget>& parent);

                DragDropWidget() = default;

            public:
                virtual ~DragDropWidget();

                static std::shared_ptr<DragDropWidget> create(
                    const std::shared_ptr<Context>&,
                    const std::vector<std::string>& text,
                    const std::shared_ptr<IWidget>& parent = nullptr);

                const std::vector<std::string>& getText() const;

                void sizeHintEvent(const SizeHintEvent&) override;
                void mouseMoveEvent(MouseMoveEvent&) override;
                void dragEnterEvent(DragDropEvent&) override;
                void dragLeaveEvent(DragDropEvent&) override;
                void dropEvent(DragDropEvent&) override;

            private:
                std::vector<std::string> _text;
                int _dragLength = 0;
                bool _dropTarget = false;;
            };

            void DragDropWidget::_init(
                const std::shared_ptr<Context>& context,
                const std::vector<std::string>& text,
                const std::shared_ptr<IWidget>& parent)
            {
                IMouseWidget::_init(context, "ftk::ui_test::DragDropWidget", parent);
                setStretch(Stretch::Expanding);
                _setMouseHoverEnabled(true);
                _setMousePressEnabled(true);
                _text = text;
            }

            DragDropWidget::~DragDropWidget()
            {}

            std::shared_ptr<DragDropWidget> DragDropWidget::create(
                const std::shared_ptr<Context>& context,
                const std::vector<std::string>& text,
                const std::shared_ptr<IWidget>& parent)
            {
                auto out = std::shared_ptr<DragDropWidget>(new DragDropWidget);
                out->_init(context, text, parent);
                return out;
            }

            const std::vector<std::string>& DragDropWidget::getText() const
            {
                return _text;
            }

            void DragDropWidget::sizeHintEvent(const SizeHintEvent& event)
            {
                IMouseWidget::sizeHintEvent(event);
                _dragLength = event.style->getSizeRole(SizeRole::DragLength, event.displayScale);
                setSizeHint(Size2I(100, 100));
            }

            void DragDropWidget::mouseMoveEvent(MouseMoveEvent& event)
            {
                IMouseWidget::mouseMoveEvent(event);
                if (_isMousePressed())
                {
                    const float length = ftk::length(event.pos - _getMousePressPos());
                    if (length > _dragLength)
                    {
                        event.dragDropData = std::make_shared<DragDropTextData>(std::vector<std::string>({ _text }));
                        const Box2I& g = getGeometry();
                        const int w = g.w();
                        const int h = g.h();
                        event.dragDropCursor = Image::create(w, h, ImageType::RGBA_U8);
                        uint8_t* p = event.dragDropCursor->getData();
                        for (int y = 0; y < h; ++y)
                        {
                            for (int x = 0; x < w; ++x)
                            {
                                p[0] = 255;
                                p[1] = 255;
                                p[2] = 255;
                                p[3] = 63;
                                p += 4;
                            }
                        }
                        event.dragDropCursorHotspot = _getMousePos() - g.min;
                    }
                }
            }

            void DragDropWidget::dragEnterEvent(DragDropEvent& event)
            {
                event.accept = true;
                _dropTarget = true;
            }

            void DragDropWidget::dragLeaveEvent(DragDropEvent& event)
            {
                event.accept = true;
                _dropTarget = false;
            }

            void DragDropWidget::dropEvent(DragDropEvent& event)
            {
                if (auto data = std::dynamic_pointer_cast<DragDropTextData>(event.data))
                {
                    event.accept = true;
                    _text = data->getText();
                }
            }
        }

        DragDropTest::DragDropTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::DragDropTest")
        {}

        DragDropTest::~DragDropTest()
        {}

        std::shared_ptr<DragDropTest> DragDropTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DragDropTest>(new DragDropTest(context));
        }
                
        void DragDropTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("DragDropTest");
                auto app = App::create(
                    context,
                    argv,
                    "DragDropTest",
                    "Drag and drop test.");
                auto window = Window::create(context, app, "DragDropTest");
                auto layout = HorizontalLayout::create(context, window);
                auto dragDropWidget0 = DragDropWidget::create(context, { "Drag 0" }, layout);
                auto dragDropWidget1 = DragDropWidget::create(context, { "Drag 1" }, layout);
                auto spacer = Spacer::create(context, Orientation::Horizontal, layout);
                spacer->setStretch(Stretch::Expanding);
                window->show();
                app->tick();
            }
        }
    }
}
