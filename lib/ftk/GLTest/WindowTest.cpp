// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GLTest/WindowTest.h>

#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

using namespace ftk::gl;

namespace ftk
{
    namespace gl_test
    {
        WindowTest::WindowTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::gl_test::WindowTest")
        {}

        WindowTest::~WindowTest()
        {}

        std::shared_ptr<WindowTest> WindowTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<WindowTest>(new WindowTest(context));
        }
        
        void WindowTest::run()
        {
            {
                // Hidden, like every other OpenGL test's window. A shown
                // one takes the pointer and the keyboard from whoever is
                // at the machine, and going full screen on it blanks the
                // display -- twice, on the way in and on the way back.
                // Nothing here reads the window off the screen.
                Size2I size(1024, 1024);
                auto window = Window::create(
                    _context,
                    "WindowTest",
                    size,
                    static_cast<int>(WindowOptions::DoubleBuffer) |
                    static_cast<int>(WindowOptions::MakeCurrent));
                FTK_CHECK(window->getID());
                _print(Format("Screen: {0}").arg(window->getScreen()));
                _print(Format("Full screen: {0}").arg(window->isFullScreen()));
                _print(Format("Float on top: {0}").arg(window->isFloatOnTop()));

                size = Size2I(512, 512);
                window->setSize(size);
                window->hide();
                window->setFullScreen(true);
                window->setFullScreen(true);
                window->setFullScreen(false);
                window->setFloatOnTop(true);
                window->setFloatOnTop(true);
                window->setFloatOnTop(false);
                window->makeCurrent();
                window->swap();
                window->clearCurrent();
            }
            {
                // The geometry a window is remembered by: its position and
                // size when it is neither maximized nor full screen.
                // Maximizing and going full screen leave it as it was, where
                // remembering the window by the size it has then brought it
                // back filling the screen without being maximized.
                const Size2I size(640, 480);
                auto window = Window::create(
                    _context,
                    "WindowTest",
                    size,
                    static_cast<int>(WindowOptions::DoubleBuffer));
                FTK_CHECK(window->getNormalGeometry().size() == window->getSize());
                FTK_CHECK(window->getNormalGeometry().min == window->getPos());

                // Not every platform lets a window be placed: Wayland keeps
                // the position to itself.
                const V2I pos = window->getPos() + V2I(20, 30);
                window->setPos(pos);
                const bool placed = window->getPos() == pos;
                _print(Format("Window placed: {0}").arg(placed));
                if (placed)
                {
                    FTK_CHECK(window->getNormalGeometry().min == pos);
                }
                const Size2I size2(512, 400);
                window->setSize(size2);
                FTK_CHECK(window->getNormalGeometry().size() == window->getSize());
                const Box2I normal = window->getNormalGeometry();

                window->setMaximized(true);
                window->geometryChanged();
                _print(Format("Maximized: {0}").arg(window->isMaximized()));
                FTK_CHECK(window->getNormalGeometry() == normal);
                window->setMaximized(false);
                window->geometryChanged();
                FTK_CHECK(window->getNormalGeometry() == normal);

                window->setFullScreen(true);
                window->geometryChanged();
                FTK_CHECK(window->getNormalGeometry() == normal);
                window->setFullScreen(false);

                // A size noted the moment before the platform says the
                // window was maximized is the maximized size, noted early.
                window->setSize(Size2I(600, 500));
                FTK_CHECK(window->getNormalGeometry().size() == window->getSize());
                window->geometryChanged(true);
                FTK_CHECK(window->getNormalGeometry().size() == normal.size());
            }
        }
    }
}

