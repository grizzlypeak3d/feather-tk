// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <ftk/Core/Timer.h>

namespace gfx
{
    class App;

    //! Game of Life widget.
    class GOLWidget : public ftk::IWidget
    {
    protected:
        void _init(const std::shared_ptr<ftk::Context>&);

        GOLWidget() = default;

    public:
        virtual ~GOLWidget();

        //! Create a new widget.
        static std::shared_ptr<GOLWidget> create(
            const std::shared_ptr<ftk::Context>&);

        void setGeometry(const ftk::Box2I&) override;
        void setVisible(bool) override;
        void sizeHintEvent(const ftk::SizeHintEvent&) override;
        void drawEvent(const ftk::Box2I&, const ftk::DrawEvent&) override;
 
    private:
        ftk::V2I _wrap(const ftk::V2I&) const;
        uint8_t _getCell(size_t index, const ftk::V2I&) const;
        void _setCell(size_t index, const ftk::V2I&, uint8_t);
        void _randomize(size_t index);
        void _tick();

        int _cellSize = 0;
        ftk::Size2I _cellsSize;
        std::array<std::vector<uint8_t>, 2> _cells;
        size_t _currentCells = 0;
        double _noiseZ = 0.0;
        std::shared_ptr<ftk::Timer> _timer;
    };
}
