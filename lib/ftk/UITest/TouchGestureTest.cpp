// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/TouchGestureTest.h>

#include <ftk/UI/TouchGesture.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

#include <cmath>

namespace ftk
{
    namespace ui_test
    {
        TouchGestureTest::TouchGestureTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::TouchGestureTest")
        {}

        TouchGestureTest::~TouchGestureTest()
        {}

        std::shared_ptr<TouchGestureTest> TouchGestureTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<TouchGestureTest>(new TouchGestureTest(context));
        }

        void TouchGestureTest::run()
        {
            _pan();
            _pinch();
            _undecided();
            _fingers();
            _mouseDelay();
        }

        void TouchGestureTest::_pan()
        {
            // Two fingers moving together, each sending its own events. The
            // distance between them grows and shrinks from one event to the
            // next, which is not a zoom: a pan keeps the zoom where it was.
            for (bool batched : { false, true })
            {
                TouchGestureRecognizer r;
                r.setThreshold(16.F);
                r.fingerDown(1, V2F(100.F, 100.F));
                FTK_CHECK(!r.isMouseBlocked());
                FTK_CHECK(r.takeGestures().empty());
                r.fingerDown(2, V2F(200.F, 100.F));
                FTK_CHECK(r.isMouseBlocked());
                auto gestures = r.takeGestures();
                FTK_CHECK(1 == gestures.size());
                FTK_CHECK(TouchGestureType::Begin == gestures[0].type);
                FTK_CHECK(V2I(150, 100) == gestures[0].pos);

                V2F pan;
                size_t updates = 0;
                for (int i = 1; i <= 10; ++i)
                {
                    r.fingerMove(1, V2F(100.F + i * 5.F, 100.F));
                    if (!batched)
                    {
                        for (const auto& gesture : r.takeGestures())
                        {
                            FTK_CHECK(1.F == gesture.zoom);
                            pan = pan + gesture.pan;
                            ++updates;
                        }
                    }
                    r.fingerMove(2, V2F(200.F + i * 5.F, 100.F));
                    for (const auto& gesture : r.takeGestures())
                    {
                        FTK_CHECK(TouchGestureType::Update == gesture.type);
                        FTK_CHECK(1.F == gesture.zoom);
                        pan = pan + gesture.pan;
                        ++updates;
                    }
                }
                _print(Format("Pan {0}: {1} in {2} updates").
                    arg(batched ? "batched" : "unbatched").arg(pan).arg(updates));
                // Nothing lost to the threshold: the pan that decided the
                // gesture arrives with it.
                FTK_CHECK(std::abs(pan.x - 50.F) < .001F);
                FTK_CHECK(0.F == pan.y);

                r.fingerUp(1);
                gestures = r.takeGestures();
                FTK_CHECK(1 == gestures.size());
                FTK_CHECK(TouchGestureType::End == gestures[0].type);
                FTK_CHECK(r.isMouseBlocked());
                FTK_CHECK(r.isTouching());
                r.fingerUp(2);
                FTK_CHECK(!r.isMouseBlocked());
                FTK_CHECK(!r.isTouching());
                FTK_CHECK(r.takeGestures().empty());
            }
        }

        void TouchGestureTest::_pinch()
        {
            // The fingers spreading: a pinch, which zooms by how far apart
            // they are from where they started.
            TouchGestureRecognizer r;
            r.setThreshold(16.F);
            r.fingerDown(1, V2F(100.F, 100.F));
            r.fingerDown(2, V2F(200.F, 100.F));
            r.takeGestures();
            float zoom = 1.F;
            for (int i = 1; i <= 10; ++i)
            {
                r.fingerMove(1, V2F(100.F - i * 5.F, 100.F));
                r.fingerMove(2, V2F(200.F + i * 5.F, 100.F));
                for (const auto& gesture : r.takeGestures())
                {
                    zoom *= gesture.zoom;
                    FTK_CHECK(V2I(150, 100) == gesture.pos);
                }
            }
            _print(Format("Pinch: {0}").arg(zoom));
            FTK_CHECK(std::abs(zoom - 2.F) < .001F);
        }

        void TouchGestureTest::_undecided()
        {
            // Fingers that settle without going anywhere move nothing.
            TouchGestureRecognizer r;
            r.setThreshold(16.F);
            r.fingerDown(1, V2F(100.F, 100.F));
            r.fingerDown(2, V2F(200.F, 100.F));
            r.takeGestures();
            r.fingerMove(1, V2F(104.F, 103.F));
            r.fingerMove(2, V2F(197.F, 102.F));
            FTK_CHECK(r.takeGestures().empty());
            r.fingerUp(2);
            const auto gestures = r.takeGestures();
            FTK_CHECK(1 == gestures.size());
            FTK_CHECK(TouchGestureType::End == gestures[0].type);
        }

        void TouchGestureTest::_fingers()
        {
            // A third finger, and one of the first two lifting, change the
            // pair the gesture follows without making it jump.
            TouchGestureRecognizer r;
            r.setThreshold(16.F);
            r.fingerDown(1, V2F(100.F, 100.F));
            r.fingerDown(2, V2F(200.F, 100.F));
            r.fingerMove(1, V2F(130.F, 100.F));
            r.fingerMove(2, V2F(230.F, 100.F));
            auto gestures = r.takeGestures();
            FTK_CHECK(2 == gestures.size());
            r.fingerDown(3, V2F(500.F, 500.F));
            FTK_CHECK(r.takeGestures().empty());
            r.fingerUp(1);
            FTK_CHECK(r.takeGestures().empty());
            r.fingerMove(2, V2F(240.F, 100.F));
            r.fingerMove(3, V2F(510.F, 500.F));
            gestures = r.takeGestures();
            FTK_CHECK(1 == gestures.size());
            FTK_CHECK(V2F(10.F, 0.F) == gestures[0].pan);
            FTK_CHECK(1.F == gestures[0].zoom);

            // Down to one finger ends it; a second finger again starts a
            // new one.
            r.fingerUp(2);
            gestures = r.takeGestures();
            FTK_CHECK(1 == gestures.size());
            FTK_CHECK(TouchGestureType::End == gestures[0].type);
            r.fingerDown(4, V2F(600.F, 500.F));
            gestures = r.takeGestures();
            FTK_CHECK(1 == gestures.size());
            FTK_CHECK(TouchGestureType::Begin == gestures[0].type);
        }

        void TouchGestureTest::_mouseDelay()
        {
            using namespace std::chrono;
            const auto t0 = steady_clock::now();

            // A tap waits for nothing: the release sends the press with it.
            {
                TouchMouseDelay d;
                d.setThreshold(16.F);
                FTK_CHECK(d.event(V2F(10.F, 10.F), false, t0));
                FTK_CHECK(d.isHolding());
                FTK_CHECK(!d.isReady(t0));
                FTK_CHECK(d.event(V2F(10.F, 10.F), true, t0 + milliseconds(20)));
                FTK_CHECK(d.isReady(t0 + milliseconds(20)));
                d.sent();
                FTK_CHECK(!d.isHolding());
            }

            // A finger that stays down goes through after the timeout, and
            // the rest of its touch goes straight through.
            {
                TouchMouseDelay d;
                d.setTimeout(milliseconds(100));
                FTK_CHECK(d.event(V2F(10.F, 10.F), false, t0));
                FTK_CHECK(!d.isReady(t0 + milliseconds(50)));
                FTK_CHECK(d.isReady(t0 + milliseconds(100)));
                d.sent();
                FTK_CHECK(!d.event(V2F(12.F, 10.F), false, t0 + milliseconds(120)));
                FTK_CHECK(!d.event(V2F(12.F, 10.F), true, t0 + milliseconds(140)));
                // And the next touch is held again.
                FTK_CHECK(d.event(V2F(50.F, 50.F), false, t0 + milliseconds(500)));
            }

            // A finger that moves past the threshold goes through at once.
            {
                TouchMouseDelay d;
                d.setThreshold(16.F);
                d.event(V2F(10.F, 10.F), false, t0);
                d.event(V2F(20.F, 10.F), false, t0 + milliseconds(10));
                FTK_CHECK(!d.isReady(t0 + milliseconds(10)));
                d.event(V2F(30.F, 10.F), false, t0 + milliseconds(20));
                FTK_CHECK(d.isReady(t0 + milliseconds(20)));
            }

            // A second finger in time drops it all, and the next touch is
            // held again.
            {
                TouchMouseDelay d;
                d.event(V2F(10.F, 10.F), false, t0);
                d.drop();
                FTK_CHECK(!d.isHolding());
                FTK_CHECK(!d.isReady(t0 + seconds(1)));
                FTK_CHECK(d.event(V2F(10.F, 10.F), false, t0 + seconds(1)));
            }
        }
    }
}
