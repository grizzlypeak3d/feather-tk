// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/DragAndDropTest.h>

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
            class DragAndDropWidget : public IMouseWidget
            {
            protected:
                void _init(
                    const std::shared_ptr<Context>&,
                    const std::string& text,
                    const std::shared_ptr<IWidget>& parent);

                DragAndDropWidget() = default;

            public:
                virtual ~DragAndDropWidget();

                static std::shared_ptr<DragAndDropWidget> create(
                    const std::shared_ptr<Context>&,
                    const std::string& text,
                    const std::shared_ptr<IWidget>& parent = nullptr);

                const std::string& getText() const;

                void sizeHintEvent(const SizeHintEvent&) override;
                void mouseMoveEvent(MouseMoveEvent&) override;
                void dragEnterEvent(DragAndDropEvent&) override;
                void dragLeaveEvent(DragAndDropEvent&) override;
                void dropEvent(DragAndDropEvent&) override;

            private:
                std::string _text;
                int _dragLength = 0;
                bool _dropTarget = false;;
            };

            void DragAndDropWidget::_init(
                const std::shared_ptr<Context>& context,
                const std::string& text,
                const std::shared_ptr<IWidget>& parent)
            {
                IMouseWidget::_init(context, "ftk::examples::dnd::DragAndDropWidget", parent);
                setStretch(Stretch::Expanding);
                _setMouseHoverEnabled(true);
                _setMousePressEnabled(true);
                _text = text;
            }

            DragAndDropWidget::~DragAndDropWidget()
            {}

            std::shared_ptr<DragAndDropWidget> DragAndDropWidget::create(
                const std::shared_ptr<Context>& context,
                const std::string& text,
                const std::shared_ptr<IWidget>& parent)
            {
                auto out = std::shared_ptr<DragAndDropWidget>(new DragAndDropWidget);
                out->_init(context, text, parent);
                return out;
            }

            const std::string& DragAndDropWidget::getText() const
            {
                return _text;
            }

            void DragAndDropWidget::sizeHintEvent(const SizeHintEvent& event)
            {
                IMouseWidget::sizeHintEvent(event);
                _dragLength = event.style->getSizeRole(SizeRole::DragLength, event.displayScale);
                _setSizeHint(Size2I(100, 100));
            }

            void DragAndDropWidget::mouseMoveEvent(MouseMoveEvent& event)
            {
                IMouseWidget::mouseMoveEvent(event);
                if (_isMousePressed())
                {
                    const float length = ftk::length(event.pos - _getMousePressPos());
                    if (length > _dragLength)
                    {
                        event.dndData = std::make_shared<TextDragAndDropData>(_text);
                        const Box2I& g = getGeometry();
                        const int w = g.w();
                        const int h = g.h();
                        event.dndCursor = Image::create(w, h, ImageType::RGBA_U8);
                        uint8_t* p = event.dndCursor->getData();
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
                        event.dndCursorHotspot = _getMousePos() - g.min;
                    }
                }
            }

            void DragAndDropWidget::dragEnterEvent(DragAndDropEvent& event)
            {
                event.accept = true;
                _dropTarget = true;
            }

            void DragAndDropWidget::dragLeaveEvent(DragAndDropEvent& event)
            {
                event.accept = true;
                _dropTarget = false;
            }

            void DragAndDropWidget::dropEvent(DragAndDropEvent& event)
            {
                if (auto data = std::dynamic_pointer_cast<TextDragAndDropData>(event.data))
                {
                    event.accept = true;
                    _text = data->getText();
                }
            }
        }

        DragAndDropTest::DragAndDropTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::DragAndDropTest")
        {}

        DragAndDropTest::~DragAndDropTest()
        {}

        std::shared_ptr<DragAndDropTest> DragAndDropTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DragAndDropTest>(new DragAndDropTest(context));
        }
                
        void DragAndDropTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("DragAndDropTest");
                auto app = App::create(
                    context,
                    argv,
                    "DragAndDropTest",
                    "Drag and drop test.");
                auto window = Window::create(context, app, "DragAndDropTest");
                auto layout = HorizontalLayout::create(context, window);
                auto dndWidget0 = DragAndDropWidget::create(context, "Drag 0", layout);
                auto dndWidget1 = DragAndDropWidget::create(context, "Drag 1", layout);
                auto spacer = Spacer::create(context, Orientation::Horizontal, layout);
                spacer->setStretch(Stretch::Expanding);
                window->show();
                app->tick();
            }
        }
    }
}
