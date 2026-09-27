// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Event.h>

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
}
