// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "GOL.h"

#include <ftk/Core/Noise.h>

using namespace ftk;

namespace gfx
{
    void GOLWidget::_init(const std::shared_ptr<Context>& context)
    {
        IWidget::_init(context, "GOLWidget", nullptr);

        setStretch(Stretch::Expanding);

        _timer = Timer::create(context);
        _timer->setRepeating(true);

        /*_timer2 = Timer::create(context);
        _timer2->setRepeating(true);
        _timer2->start(
            std::chrono::milliseconds(10 * 1000),
            [this]
            {
                _randomize(_currentCells);
            });*/
    }

    GOLWidget::~GOLWidget()
    {}

    std::shared_ptr<GOLWidget> GOLWidget::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<GOLWidget>(new GOLWidget);
        out->_init(context);
        return out;
    }

    void GOLWidget::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IWidget::setGeometry(value);
        if (changed)
        {
            const Box2I& g = getGeometry();
            _cellsSize.w = g.w() / _cellSize;
            _cellsSize.h = g.h() / _cellSize;
            const size_t size = _cellsSize.w * _cellsSize.h;
            _cells[0].resize(size);
            _cells[1].resize(size);
            _randomize(_currentCells);
        }
    }

    void GOLWidget::setVisible(bool value)
    {
        const bool changed = value != isVisible(false);
        IWidget::setVisible(value);
        if (changed)
        {
            if (value)
            {
                _timer->start(
                    std::chrono::milliseconds(1000 / 24),
                    [this]
                    {
                        _tick();
                    });
            }
            else
            {
                _timer->stop();
            }
        }
    }

    void GOLWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        _cellSize = 3 * event.displayScale;
    }

    void GOLWidget::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        const Box2I& g = getGeometry();
        event.render->drawRect(g, Color4F(0.F, 0.F, 0.F));
        std::vector<Box2F> rects;
        V2I pos;
        for (; pos.y < _cellsSize.h; ++pos.y)
        {
            for (pos.x = 0; pos.x < _cellsSize.w; ++pos.x)
            {
                const uint8_t v = _getCell(_currentCells, pos);
                if (v)
                {
                    rects.push_back(Box2F(
                        g.min.x + pos.x * _cellSize,
                        g.min.y + pos.y * _cellSize,
                        _cellSize - 1,
                        _cellSize - 1));
                }
            }
        }
        event.render->drawRects(rects, Color4F(1.F, 1.F, 1.F));
    }

    V2I GOLWidget::_wrap(const V2I& pos) const
    {
        V2I out = pos;
        if (out.x < 0)
        {
            out.x = _cellsSize.w - 1;
        }
        else if (out.x >= _cellsSize.w)
        {
            out.x = 0;
        }
        if (out.y < 0)
        {
            out.y = _cellsSize.h - 1;
        }
        else if (out.y >= _cellsSize.h)
        {
            out.y = 0;
        }
        return out;
    }

    uint8_t GOLWidget::_getCell(size_t index, const V2I& pos) const
    {
        const V2I pos2 = _wrap(pos);
        return _cells[index][pos2.y * _cellsSize.w + pos2.x];
    }

    void GOLWidget::_setCell(size_t index, const V2I& pos, uint8_t value)
    {
        const V2I pos2 = _wrap(pos);
        _cells[index][pos2.y * _cellsSize.w + pos2.x] = value;
    }

    void GOLWidget::_randomize(size_t index)
    {
        Noise noise;
        V2I pos;
        for (; pos.y < _cellsSize.h; pos.y = ++pos.y)
        {
            for (pos.x = 0; pos.x < _cellsSize.w; pos.x = ++pos.x)
            {
                const double n = noise.get(pos.x / 10.0, pos.y / 10.0, _noiseZ / 10.0);
                if (n > 0.1)
                {
                    _setCell(index, pos, 255);
                }
            }
        }
        setDrawUpdate();
    }

    void GOLWidget::_tick()
    {
        const size_t source = _currentCells;
        const size_t dest = (_currentCells + 1) % 2;
        const std::vector<V2I> offsets =
        {
            { -1, -1 }, { 0, -1 }, { 1, -1 },
            { -1,  0 },            { 1,  0 },
            { -1,  1 }, { 0,  1 }, { 1,  1 }
        };
        V2I pos;
        for (; pos.y < _cellsSize.h; ++pos.y)
        {
            for (pos.x = 0; pos.x < _cellsSize.w; ++pos.x)
            {
                uint8_t cell = _getCell(source, pos);
                size_t neighborsAlive = 0;
                for (const auto& offset : offsets)
                {
                    if (_getCell(source, pos + offset))
                    {
                        ++neighborsAlive;
                    }
                }
                if (cell)
                {
                    if (neighborsAlive < 2)
                    {
                        cell = 0;
                    }
                    else if (neighborsAlive > 3)
                    {
                        cell = 0;
                    }
                }
                else
                {
                    if (3 == neighborsAlive)
                    {
                        cell = 255;
                    }
                }
                _setCell(dest, pos, cell);
            }
        }
        _currentCells = dest;
        _noiseZ += 1.0;
        setDrawUpdate();
    }
}
