// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ContextMenuTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IPopup.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/Menu.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Spacer.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        namespace
        {
            //! Window that exposes the mouse input the application
            //! normally feeds in from the window system.
            class TestWindow : public Window
            {
            protected:
                TestWindow() = default;

            public:
                static std::shared_ptr<TestWindow> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<App>& app,
                    const std::string& title,
                    const Size2I& size)
                {
                    auto out = std::shared_ptr<TestWindow>(new TestWindow);
                    out->_init(context, app, title, size);
                    return out;
                }

                //! Lay the window out, without depending on the window
                //! system to deliver a resize. Until this happens the
                //! window and everything in it has an empty geometry, and
                //! nothing is under the cursor.
                void resize(const Size2I& size)
                {
                    _setSize(size, size);
                }

                void click(const V2I& pos, MouseButton button)
                {
                    _cursorEnter(true);
                    _cursorPos(pos);
                    _mouseButton(button, true, 0);
                    _mouseButton(button, false, 0);
                }

                //! Get the menus the window is currently showing.
                std::vector<std::shared_ptr<IMenuPopup> > getPopups() const
                {
                    std::vector<std::shared_ptr<IMenuPopup> > out;
                    for (const auto& child : getChildren())
                    {
                        if (auto popup = std::dynamic_pointer_cast<IMenuPopup>(child))
                        {
                            out.push_back(popup);
                        }
                    }
                    return out;
                }
            };

            //! Widget that uses the right button itself, the way a viewport
            //! bound to right drag does.
            class ClaimWidget : public IMouseWidget
            {
            protected:
                ClaimWidget() = default;

            public:
                static std::shared_ptr<ClaimWidget> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<IWidget>& parent = nullptr)
                {
                    auto out = std::shared_ptr<ClaimWidget>(new ClaimWidget);
                    out->_init(context, "ftk::ui_test::ClaimWidget", parent);
                    return out;
                }

                bool pressed = false;

                Size2I getSizeHint() const override { return Size2I(100, 100); }

                void mousePressEvent(MouseClickEvent& event) override
                {
                    IMouseWidget::mousePressEvent(event);
                    if (MouseButton::Right == event.button)
                    {
                        event.accept = true;
                        pressed = true;
                    }
                }
            };

            V2I center(const std::shared_ptr<IWidget>& widget)
            {
                const Box2I& g = widget->getGeometry();
                return V2I(g.min.x + g.w() / 2, g.min.y + g.h() / 2);
            }
        }

        ContextMenuTest::ContextMenuTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ContextMenuTest")
        {}

        ContextMenuTest::~ContextMenuTest()
        {}

        std::shared_ptr<ContextMenuTest> ContextMenuTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ContextMenuTest>(new ContextMenuTest(context));
        }

        void ContextMenuTest::run()
        {
            const Size2I windowSize(1280, 960);
            std::vector<std::string> argv;
            argv.push_back("ContextMenuTest");
            auto app = App::create(
                _context,
                argv,
                "ContextMenuTest",
                "Context menu test.");
            auto window = TestWindow::create(
                _context,
                app,
                "ContextMenuTest",
                windowSize);
            auto layout = VerticalLayout::create(_context, window);
            auto lineEdit = LineEdit::create(_context, layout);

            // A spacer with no callback of its own, inside a layout that
            // has one, to exercise propagation up the hierarchy.
            int layoutMenuCount = 0;
            auto spacer = Spacer::create(_context, Orientation::Vertical, layout);
            spacer->setStretch(Stretch::Expanding);
            layout->setContextMenuCallback(
                [this, &layoutMenuCount]
                {
                    ++layoutMenuCount;
                    auto out = Menu::create(_context);
                    out->addAction(Action::create("Layout Action", [] {}));
                    return out;
                });

            window->show();
            window->resize(windowSize);
            app->tick();

            // A left click does not open a menu.
            window->click(center(lineEdit), MouseButton::Left);
            app->tick();
            FTK_CHECK(window->getPopups().empty());

            // A right click on the line edit opens its text menu; the menu
            // itself holds the key focus while it is up.
            window->click(center(lineEdit), MouseButton::Right);
            app->tick();
            FTK_CHECK(1 == window->getPopups().size());
            FTK_CHECK(window->getPopups().front()->isOpen());
            FTK_CHECK(0 == layoutMenuCount);
            {
                auto menu = std::dynamic_pointer_cast<Menu>(
                    window->getPopups().front());
                FTK_CHECK(menu);
                const std::vector<std::string> text =
                {
                    "Undo", "Redo", "Cut", "Copy", "Paste", "Select All"
                };
                FTK_CHECK(text.size() == menu->getActions().size());
                for (size_t i = 0; i < text.size(); ++i)
                {
                    FTK_CHECK(text[i] == menu->getActions()[i]->getText());
                }
            }

            // Closing the menu removes it from the window and returns the
            // key focus to the line edit, which the menu took before
            // opening so that it acts on the right widget.
            window->getPopups().front()->close();
            app->tick();
            FTK_CHECK(window->getPopups().empty());
            FTK_CHECK(lineEdit->hasKeyFocus());

            // A right click on a widget without a callback walks up to the
            // first ancestor that has one.
            window->click(center(spacer), MouseButton::Right);
            app->tick();
            FTK_CHECK(1 == window->getPopups().size());
            FTK_CHECK(1 == layoutMenuCount);
            window->getPopups().front()->close();
            app->tick();

            // A callback returning null falls through to the ancestor.
            spacer->setContextMenuCallback([] { return nullptr; });
            window->click(center(spacer), MouseButton::Right);
            app->tick();
            FTK_CHECK(1 == window->getPopups().size());
            FTK_CHECK(2 == layoutMenuCount);
            window->getPopups().front()->close();
            app->tick();

            // A widget that uses the right button for something else keeps
            // it by accepting the press; its ancestor's menu stays shut.
            spacer->setContextMenuCallback(nullptr);
            auto claim = ClaimWidget::create(_context, layout);
            claim->setStretch(Stretch::Expanding);
            app->tick();
            window->click(center(claim), MouseButton::Right);
            app->tick();
            FTK_CHECK(claim->pressed);
            FTK_CHECK(window->getPopups().empty());
            FTK_CHECK(2 == layoutMenuCount);
            claim->setParent(nullptr);
            app->tick();

            // An empty menu is not worth showing.
            layout->setContextMenuCallback(
                [this] { return Menu::create(_context); });
            window->click(center(spacer), MouseButton::Right);
            app->tick();
            FTK_CHECK(window->getPopups().empty());

            // Opening at a position works near the window edges, where the
            // menu has to flip to stay on screen.
            auto menu = Menu::create(_context);
            menu->addAction(Action::create("Action", [] {}));
            menu->open(window, V2I(windowSize.w - 1, windowSize.h - 1));
            app->tick();
            FTK_CHECK(1 == window->getPopups().size());
            menu->close();
            app->tick();
            FTK_CHECK(window->getPopups().empty());
        }
    }
}
