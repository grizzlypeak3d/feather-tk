// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/TouchGesture.h>

#include <algorithm>
#include <cmath>

namespace ftk
{
    void TouchGestureRecognizer::setThreshold(float value)
    {
        _threshold = value;
    }

    void TouchGestureRecognizer::fingerDown(uint64_t id, const V2F& pos)
    {
        _flush();
        const auto i = std::find_if(
            _fingers.begin(),
            _fingers.end(),
            [id](const std::pair<uint64_t, V2F>& value)
            {
                return value.first == id;
            });
        if (i != _fingers.end())
            return;
        _fingers.push_back(std::make_pair(id, pos));
        if (!_active && _fingers.size() >= 2)
        {
            _active = true;
            _mouseBlocked = true;
            _mode = Mode::Undecided;
            _pair(_startCenter, _startDistance);
            _lastCenter = _startCenter;
            _lastDistance = _startDistance;
            TouchGesture gesture;
            gesture.type = TouchGestureType::Begin;
            gesture.pos = V2I(std::round(_startCenter.x), std::round(_startCenter.y));
            _gestures.push_back(gesture);
        }
        else if (_active)
        {
            _rebase();
        }
    }

    void TouchGestureRecognizer::fingerMove(uint64_t id, const V2F& pos)
    {
        const auto i = std::find_if(
            _fingers.begin(),
            _fingers.end(),
            [id](const std::pair<uint64_t, V2F>& value)
            {
                return value.first == id;
            });
        if (i != _fingers.end() && i->second != pos)
        {
            i->second = pos;
            _moved = true;
        }
    }

    void TouchGestureRecognizer::fingerUp(uint64_t id)
    {
        _flush();
        const auto i = std::find_if(
            _fingers.begin(),
            _fingers.end(),
            [id](const std::pair<uint64_t, V2F>& value)
            {
                return value.first == id;
            });
        if (i == _fingers.end())
            return;
        _fingers.erase(i);
        if (_active)
        {
            if (_fingers.size() < 2)
            {
                _active = false;
                TouchGesture gesture;
                gesture.type = TouchGestureType::End;
                gesture.pos = V2I(std::round(_lastCenter.x), std::round(_lastCenter.y));
                _gestures.push_back(gesture);
            }
            else
            {
                _rebase();
            }
        }
        if (_fingers.empty())
        {
            _mouseBlocked = false;
        }
    }

    std::vector<TouchGesture> TouchGestureRecognizer::takeGestures()
    {
        _flush();
        std::vector<TouchGesture> out;
        std::swap(out, _gestures);
        return out;
    }

    bool TouchGestureRecognizer::isTouching() const
    {
        return !_fingers.empty();
    }

    bool TouchGestureRecognizer::isMouseBlocked() const
    {
        return _mouseBlocked;
    }

    void TouchGestureRecognizer::_pair(V2F& center, float& distance) const
    {
        const V2F& a = _fingers[0].second;
        const V2F& b = _fingers[1].second;
        center = V2F((a.x + b.x) / 2.F, (a.y + b.y) / 2.F);
        distance = length(b - a);
    }

    void TouchGestureRecognizer::_flush()
    {
        if (_active && _moved && _fingers.size() >= 2)
        {
            V2F center;
            float distance = 0.F;
            _pair(center, distance);
            if (Mode::Undecided == _mode)
            {
                if (std::abs(distance - _startDistance) > _threshold)
                {
                    _mode = Mode::Zoom;
                }
                else if (length(center - _startCenter) > _threshold)
                {
                    _mode = Mode::Pan;
                }
            }
            if (_mode != Mode::Undecided)
            {
                // From the last event, or from the start when the gesture
                // has just been decided, so the movement that decided it is
                // not lost.
                TouchGesture gesture;
                gesture.pos = V2I(std::round(center.x), std::round(center.y));
                gesture.pan = center - _lastCenter;
                gesture.zoom =
                    Mode::Zoom == _mode && _lastDistance > 0.F && distance > 0.F ?
                    (distance / _lastDistance) :
                    1.F;
                _gestures.push_back(gesture);
                _lastCenter = center;
                _lastDistance = distance;
            }
        }
        _moved = false;
    }

    void TouchGestureRecognizer::_rebase()
    {
        // A finger coming or going moves the pair the gesture follows: the
        // gesture carries on from where the new pair is, without a jump.
        if (_fingers.size() >= 2)
        {
            _pair(_lastCenter, _lastDistance);
            if (Mode::Undecided == _mode)
            {
                _startCenter = _lastCenter;
                _startDistance = _lastDistance;
            }
        }
    }

    void TouchMouseDelay::setTimeout(const std::chrono::steady_clock::duration& value)
    {
        _timeout = value;
    }

    void TouchMouseDelay::setThreshold(float value)
    {
        _threshold = value;
    }

    bool TouchMouseDelay::event(
        const V2F& pos,
        bool release,
        const std::chrono::steady_clock::time_point& now)
    {
        bool out = true;
        switch (_state)
        {
        case State::Idle:
            _state = State::Holding;
            _start = now;
            _startPos = pos;
            _ready = release;
            _released = release;
            break;
        case State::Holding:
            if (release)
            {
                _ready = true;
                _released = true;
            }
            else if (length(pos - _startPos) > _threshold)
            {
                _ready = true;
            }
            break;
        case State::Passing:
            out = false;
            if (release)
            {
                _state = State::Idle;
            }
            break;
        }
        return out;
    }

    bool TouchMouseDelay::isReady(const std::chrono::steady_clock::time_point& now) const
    {
        return State::Holding == _state && (_ready || now - _start >= _timeout);
    }

    void TouchMouseDelay::sent()
    {
        _state = _released ? State::Idle : State::Passing;
        _ready = false;
        _released = false;
    }

    void TouchMouseDelay::drop()
    {
        _state = State::Idle;
        _ready = false;
        _released = false;
    }

    bool TouchMouseDelay::isHolding() const
    {
        return State::Holding == _state;
    }
}
