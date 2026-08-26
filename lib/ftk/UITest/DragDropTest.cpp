// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/DragDropTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>
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

                Size2I getSizeHint() const override;
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

            Size2I DragDropWidget::getSizeHint() const
            {
                return Size2I(100, 100);
            }
            
            void DragDropWidget::sizeHintEvent(const SizeHintEvent& event)
            {
                IMouseWidget::sizeHintEvent(event);
                _dragLength = event.style->getSizeRole(SizeRole::DragLength, event.displayScale);
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

            // Something tall to scroll through.
            class TallWidget : public IWidget
            {
            protected:
                TallWidget() = default;

            public:
                virtual ~TallWidget() {}

                static std::shared_ptr<TallWidget> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<IWidget>& parent = nullptr)
                {
                    auto out = std::shared_ptr<TallWidget>(new TallWidget);
                    out->_init(context, "ftk::ui_test::TallWidget", parent);
                    return out;
                }

                Size2I getSizeHint() const override
                {
                    return Size2I(100, 2000);
                }
            };
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
            {
                std::vector<std::string> argv;
                argv.push_back("DragDropTest");
                auto app = App::create(
                    _context,
                    argv,
                    "DragDropTest",
                    "Drag and drop test.");
                auto window = Window::create(_context, app, "DragDropTest");
                auto layout = HorizontalLayout::create(_context, window);
                auto dragDropWidget0 = DragDropWidget::create(_context, { "Drag 0" }, layout);
                auto dragDropWidget1 = DragDropWidget::create(_context, { "Drag 1" }, layout);
                auto spacer = Spacer::create(_context, Orientation::Horizontal, layout);
                spacer->setStretch(Stretch::Expanding);
                window->show();
                app->tick();
            }

            // Drag scroll: a scroll widget with it enabled scrolls while a
            // drag hovers near an edge -- on either side of it -- and not
            // while the drag is over the middle.
            {
                std::vector<std::string> argv;
                argv.push_back("DragDropTest");
                auto app = App::create(
                    _context,
                    argv,
                    "DragDropTest",
                    "Drag and drop test.");
                auto window = Window::create(_context, app, "DragDropTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setSpacingRole(SizeRole::None);
                auto source = DragDropWidget::create(_context, { "Drag" }, layout);
                source->setStretch(Stretch::Fixed);
                auto scrollWidget = ScrollWidget::create(
                    _context, ScrollType::Vertical, layout);
                scrollWidget->setBorder(false);
                scrollWidget->setStretch(Stretch::Expanding);
                scrollWidget->setDragScroll(true);
                scrollWidget->setWidget(TallWidget::create(_context));
                window->show();
                app->tick();
                window->layout(Size2I(200, 400));
                // The layout lands on the next tick; before it everything
                // still has an empty geometry.
                app->tick();

                // Hold a drag at a point, tick, and report how far the
                // scroll widget moved. The scrolling happens in the ticks:
                // the cursor is resting, which is exactly the case drag
                // events cannot drive.
                const V2I start = center(source->getGeometry());
                auto dragTo = [&](const V2I& pos)
                    {
                        scrollWidget->setScrollPos(V2I());
                        window->drag({ start, pos }, 0, false);
                        for (int i = 0; i < 10; ++i)
                        {
                            app->tick();
                        }
                        const int out = scrollWidget->getScrollPos().y;
                        window->drag({ pos, pos + V2I(1, 0) });
                        return out;
                    };

                const Box2I& scrollGeom = scrollWidget->getGeometry();
                const int x = center(scrollGeom).x;

                // Hovering near the bottom edge, inside.
                FTK_CHECK(dragTo(V2I(x, scrollGeom.max.y - 2)) > 0);

                // Hovering past the bottom edge, outside the widget.
                FTK_CHECK(dragTo(V2I(x, scrollGeom.max.y + 4)) > 0);

                // Hovering over the middle scrolls nothing.
                FTK_CHECK(0 == dragTo(center(scrollGeom)));
            }
        }
    }
}
