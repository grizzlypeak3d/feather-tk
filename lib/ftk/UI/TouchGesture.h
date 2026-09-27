// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Event.h>

#include <chrono>
#include <cstdint>
#include <utility>
#include <vector>

namespace ftk
{
    //! Touch gesture type.
    enum class FTK_UI_API_TYPE TouchGestureType
    {
        Begin,
        Update,
        End
    };

    //! Touch gesture.
    struct FTK_UI_API_TYPE TouchGesture
    {
        TouchGestureType type = TouchGestureType::Update;
        V2I              pos;
        V2F              pan;
        float            zoom = 1.F;
    };

    //! Turns the fingers on a touch screen into gestures: two fingers pan,
    //! and pinch to zoom.
    //!
    //! Each finger sends its own events, so the movement is gathered until
    //! the gestures are taken: two fingers moving together move as one,
    //! rather than stepping apart and back together, which reads as a zoom
    //! in and out. And a gesture is a pan or a pinch from the start: until
    //! the fingers have moved past a threshold nothing moves, and then
    //! whichever changed first decides. A pan keeps the zoom where it was; a
    //! pinch zooms and pans.
    class FTK_UI_API_TYPE TouchGestureRecognizer
    {
    public:
        //! Set how far, in pixels, the fingers move before a gesture is a
        //! pan or a pinch.
        FTK_UI_API void setThreshold(float);

        //! A finger touched, at a position in pixels.
        FTK_UI_API void fingerDown(uint64_t id, const V2F&);

        //! A finger moved.
        FTK_UI_API void fingerMove(uint64_t id, const V2F&);

        //! A finger lifted.
        FTK_UI_API void fingerUp(uint64_t id);

        //! Take the gestures since the last time.
        FTK_UI_API std::vector<TouchGesture> takeGestures();

        //! Whether any finger is touching.
        FTK_UI_API bool isTouching() const;

        //! Whether the mouse made of the first finger is to be ignored: from
        //! the second finger touching until every finger has lifted.
        FTK_UI_API bool isMouseBlocked() const;

    private:
        enum class Mode
        {
            Undecided,
            Pan,
            Zoom
        };

        void _pair(V2F& center, float& distance) const;
        void _flush();
        void _rebase();

        float _threshold = 16.F;
        std::vector<std::pair<uint64_t, V2F> > _fingers;
        bool _active = false;
        bool _moved = false;
        bool _mouseBlocked = false;
        Mode _mode = Mode::Undecided;
        V2F _startCenter;
        float _startDistance = 0.F;
        V2F _lastCenter;
        float _lastDistance = 0.F;
        std::vector<TouchGesture> _gestures;
    };

    //! Holds back, briefly, the mouse a touch screen makes of the first
    //! finger.
    //!
    //! The first finger of a two finger gesture is the mouse until the
    //! second touches, and in that time it hovers, presses, selects, and
    //! drags whatever is under it. Held back, the second finger touches
    //! first, and the mouse is dropped without any of that. The events go
    //! through once the finger lifts, which is a tap and waits for nothing,
    //! or moves past the threshold, or the timeout passes; after that the
    //! rest of the touch goes straight through.
    class FTK_UI_API_TYPE TouchMouseDelay
    {
    public:
        //! Set how long the events are held.
        FTK_UI_API void setTimeout(const std::chrono::steady_clock::duration&);

        //! Set how far, in pixels, the finger moves before the events go
        //! through.
        FTK_UI_API void setThreshold(float);

        //! A mouse event made from a touch, at a position in pixels. Returns
        //! whether it is to be held.
        FTK_UI_API bool event(
            const V2F& pos,
            bool release,
            const std::chrono::steady_clock::time_point&);

        //! Whether the held events are to go through now.
        FTK_UI_API bool isReady(const std::chrono::steady_clock::time_point&) const;

        //! The held events went through.
        FTK_UI_API void sent();

        //! The held events were dropped: a gesture started.
        FTK_UI_API void drop();

        //! Whether events are being held.
        FTK_UI_API bool isHolding() const;

    private:
        enum class State
        {
            Idle,
            Holding,
            Passing
        };

        std::chrono::steady_clock::duration _timeout = std::chrono::milliseconds(100);
        float _threshold = 16.F;
        State _state = State::Idle;
        std::chrono::steady_clock::time_point _start;
        V2F _startPos;
        bool _ready = false;
        bool _released = false;
    };
}
