// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/TableWidget.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/ScrollWidget.h>

namespace ftk
{
    TableCell::TableCell(
        const std::string& text,
        bool editable,
        ColorRole colorRole) :
        text(text),
        editable(editable),
        colorRole(colorRole)
    {}

    TableRow::TableRow(
        const std::vector<TableCell>& cells,
        bool heading) :
        cells(cells),
        heading(heading)
    {}

    TableIndex::TableIndex(int row, int column) :
        row(row),
        column(column)
    {}

    bool TableIndex::isValid() const
    {
        return row >= 0 && column >= 0;
    }

    struct TableWidget::Private
    {
        std::vector<TableRow> rows;
        int columnCount = 0;
        std::vector<bool> columnStretch;
        TableIndex current;
        TableIndex hover;
        std::function<void(const TableIndex&)> callback;
        std::shared_ptr<IWidget> editor;
        TableIndex editorIndex;
        bool editorHadFocus = false;
        bool editorClose = false;
        SizeRole marginRole = SizeRole::None;
        ColorRole headingRole = ColorRole::Base;
        bool columnLines = false;

        struct SizeData
        {
            bool init = true;
            int margin = 0;
            int cellMargin = 0;
            int keyFocus = 0;
            int pad = 0;
            int headingSpace = 0;
            int border = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            FontInfo headingFontInfo;
            FontMetrics headingFontMetrics;
            int rowHeight = 0;
            std::vector<int> columnWidths;
            Size2I sizeHint;
        };
        SizeData size;

        struct GeomData
        {
            bool init = true;
            std::vector<int> columnX;
            std::vector<int> columnW;
            std::vector<int> rowY;
        };
        GeomData geom;

        struct DrawData
        {
            std::vector<std::vector<std::vector<std::shared_ptr<Glyph> > > > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void TableWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::TableWidget", parent);
        setAcceptsKeyFocus(true);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);
    }

    TableWidget::TableWidget() :
        _p(new Private)
    {}

    TableWidget::~TableWidget()
    {}

    std::shared_ptr<TableWidget> TableWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<TableWidget>(new TableWidget);
        out->_init(context, parent);
        return out;
    }

    const std::vector<TableRow>& TableWidget::getRows() const
    {
        return _p->rows;
    }

    void TableWidget::setRows(const std::vector<TableRow>& value)
    {
        FTK_P();
        if (value == p.rows)
            return;
        p.rows = value;
        p.columnCount = 0;
        for (const auto& row : p.rows)
        {
            p.columnCount = std::max(p.columnCount, static_cast<int>(row.cells.size()));
        }
        p.columnStretch.resize(p.columnCount, false);
        if (!_isEditable(p.current))
        {
            p.current = TableIndex();
        }
        p.hover = TableIndex();
        if (p.editor && !_isEditable(p.editorIndex))
        {
            _editorRemove();
        }
        p.size.init = true;
        p.geom.init = true;
        p.draw.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    int TableWidget::getColumnCount() const
    {
        return _p->columnCount;
    }

    bool TableWidget::getColumnStretch(int column) const
    {
        FTK_P();
        return column >= 0 && column < static_cast<int>(p.columnStretch.size()) ?
            p.columnStretch[column] :
            false;
    }

    void TableWidget::setColumnStretch(int column, bool value)
    {
        FTK_P();
        if (column < 0 || value == getColumnStretch(column))
            return;
        if (column >= static_cast<int>(p.columnStretch.size()))
        {
            p.columnStretch.resize(column + 1, false);
        }
        p.columnStretch[column] = value;
        p.geom.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    const TableIndex& TableWidget::getCurrent() const
    {
        return _p->current;
    }

    void TableWidget::setCurrent(const TableIndex& value)
    {
        FTK_P();
        const TableIndex tmp = _isEditable(value) ? value : TableIndex();
        if (tmp == p.current)
            return;
        p.current = tmp;
        setDrawUpdate();
    }

    void TableWidget::setCallback(const std::function<void(const TableIndex&)>& value)
    {
        _p->callback = value;
    }

    Box2I TableWidget::getCellRect(const TableIndex& value) const
    {
        FTK_P();
        Box2I out(0, 0, 0, 0);
        if (!p.geom.init &&
            value.row >= 0 &&
            value.row < static_cast<int>(p.geom.rowY.size()) &&
            value.column >= 0 &&
            value.column < static_cast<int>(p.geom.columnX.size()) &&
            p.rows[value.row].visible)
        {
            out = Box2I(
                p.geom.columnX[value.column],
                p.geom.rowY[value.row],
                p.geom.columnW[value.column],
                p.size.rowHeight);
        }
        return out;
    }

    const std::shared_ptr<IWidget>& TableWidget::getEditor() const
    {
        return _p->editor;
    }

    const TableIndex& TableWidget::getEditorIndex() const
    {
        return _p->editorIndex;
    }

    void TableWidget::openEditor(
        const TableIndex& index,
        const std::shared_ptr<IWidget>& widget)
    {
        FTK_P();
        if (p.editor)
        {
            p.editor->setParent(nullptr);
            p.editor.reset();
        }
        p.editorIndex = TableIndex();
        p.editorHadFocus = false;
        p.editorClose = false;
        if (widget && _isEditable(index))
        {
            p.editor = widget;
            p.editorIndex = index;
            p.editor->setParent(shared_from_this());
            _editorUpdate();
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    void TableWidget::closeEditor()
    {
        FTK_P();
        if (p.editor)
        {
            p.editorClose = true;
        }
    }

    SizeRole TableWidget::getMarginRole() const
    {
        return _p->marginRole;
    }

    void TableWidget::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        p.size.init = true;
        p.geom.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    bool TableWidget::hasColumnLines() const
    {
        return _p->columnLines;
    }

    void TableWidget::setColumnLines(bool value)
    {
        FTK_P();
        if (value == p.columnLines)
            return;
        p.columnLines = value;
        setDrawUpdate();
    }

    ColorRole TableWidget::getHeadingRole() const
    {
        return _p->headingRole;
    }

    void TableWidget::setHeadingRole(ColorRole value)
    {
        FTK_P();
        if (value == p.headingRole)
            return;
        p.headingRole = value;
        setDrawUpdate();
    }

    Size2I TableWidget::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void TableWidget::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.geom.init = true;
        }
        _geomUpdate();
        _editorUpdate();
    }

    void TableWidget::tickEvent(
        bool parentsVisible,
        bool parentsEnabled,
        const TickEvent& event)
    {
        IMouseWidget::tickEvent(parentsVisible, parentsEnabled, event);
        FTK_P();
        if (p.editor)
        {
            // The editor is closed when the key focus leaves it, once it
            // has had it.
            if (p.editor->containsKeyFocus())
            {
                p.editorHadFocus = true;
            }
            else if (p.editorHadFocus)
            {
                p.editorClose = true;
            }
            if (p.editorClose)
            {
                _editorRemove();
            }
        }
    }

    void TableWidget::styleEvent(const StyleEvent& event)
    {
        IMouseWidget::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
            p.geom.init = true;
            p.draw.reset();
        }
    }

    void TableWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
            p.size.cellMargin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.pad = event.style->getSizeRole(SizeRole::LabelPad, event.displayScale);
            p.size.headingSpace = event.style->getSizeRole(SizeRole::Spacing, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.fontInfo = event.style->getFont(FontType::Regular, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.headingFontInfo = event.style->getFont(FontType::Bold, event.displayScale);
            p.size.headingFontMetrics = event.fontSystem->getMetrics(p.size.headingFontInfo);

            // The rows are all one height, with room for a key focus
            // border around a cell.
            const int cellMargin = p.size.cellMargin + p.size.keyFocus;
            p.size.rowHeight =
                std::max(p.size.fontMetrics.lineHeight, p.size.headingFontMetrics.lineHeight) +
                cellMargin * 2;

            // A column is as wide as its widest cell, hidden rows
            // included so that the columns stay put while rows are
            // filtered. The text of a heading can run across columns.
            p.size.columnWidths.assign(p.columnCount, 0);
            int headingWidth = 0;
            for (const auto& row : p.rows)
            {
                for (size_t i = 0; i < row.cells.size(); ++i)
                {
                    const auto& text = row.cells[i].text;
                    if (text.empty())
                        continue;
                    if (row.heading)
                    {
                        headingWidth = std::max(
                            headingWidth,
                            event.fontSystem->getSize(text, p.size.headingFontInfo).w);
                    }
                    else
                    {
                        p.size.columnWidths[i] = std::max(
                            p.size.columnWidths[i],
                            event.fontSystem->getSize(text, p.size.fontInfo).w);
                    }
                }
            }
            p.size.sizeHint = Size2I();
            for (auto& i : p.size.columnWidths)
            {
                i += (cellMargin + p.size.pad) * 2;
                p.size.sizeHint.w += i;
            }
            p.size.sizeHint.w = std::max(
                p.size.sizeHint.w,
                headingWidth + (cellMargin + p.size.pad) * 2);

            bool first = true;
            for (const auto& row : p.rows)
            {
                if (row.visible)
                {
                    if (row.heading && !first)
                    {
                        p.size.sizeHint.h += p.size.headingSpace;
                    }
                    p.size.sizeHint.h += p.size.rowHeight;
                    first = false;
                }
            }
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin);

            p.geom.init = true;
            p.draw.reset();
        }
    }

    void TableWidget::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();
        _geomUpdate();
        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->glyphs.resize(p.rows.size());
        }

        const Box2I g = margin(getGeometry(), -p.size.margin);
        const int textOffset = p.size.cellMargin + p.size.keyFocus;
        const bool keyFocus = hasKeyFocus();
        for (size_t i = 0; i < p.rows.size() && i < p.geom.rowY.size(); ++i)
        {
            const auto& row = p.rows[i];
            const Box2I rowRect(g.min.x, p.geom.rowY[i], g.w(), p.size.rowHeight);
            if (!row.visible || !intersects(rowRect, drawRect))
                continue;

            if (row.heading && p.headingRole != ColorRole::None)
            {
                event.render->drawRect(
                    rowRect,
                    event.style->getColorRole(p.headingRole));
            }

            auto& glyphs = p.draw->glyphs[i];
            glyphs.resize(row.cells.size());
            for (size_t j = 0; j < row.cells.size(); ++j)
            {
                const auto& cell = row.cells[j];
                const TableIndex index(static_cast<int>(i), static_cast<int>(j));
                const Box2I cellRect(
                    p.geom.columnX[j],
                    p.geom.rowY[i],
                    p.geom.columnW[j],
                    p.size.rowHeight);

                if (p.columnLines && !row.heading && j > 0)
                {
                    event.render->drawRect(
                        Box2I(cellRect.min.x, cellRect.min.y, p.size.border, cellRect.h()),
                        event.style->getColorRole(ColorRole::Border));
                }
                if (cell.colorRole != ColorRole::None)
                {
                    Color4F color = event.style->getColorRole(cell.colorRole);
                    color.a *= .4F;
                    event.render->drawRect(
                        margin(cellRect, -p.size.keyFocus),
                        color);
                }
                if (index == p.hover && !p.editor)
                {
                    event.render->drawRect(
                        cellRect,
                        event.style->getColorRole(ColorRole::Hover));
                }
                if (index == p.current && keyFocus)
                {
                    event.render->drawMesh(
                        border(cellRect, p.size.keyFocus),
                        event.style->getColorRole(ColorRole::KeyFocus));
                }

                if (!cell.text.empty() && !(p.editor && index == p.editorIndex))
                {
                    if (glyphs[j].empty())
                    {
                        glyphs[j] = event.fontSystem->getGlyphs(
                            cell.text,
                            row.heading ? p.size.headingFontInfo : p.size.fontInfo);
                    }
                    event.render->drawText(
                        glyphs[j],
                        row.heading ? p.size.headingFontMetrics : p.size.fontMetrics,
                        V2I(cellRect.min.x + textOffset + p.size.pad,
                            cellRect.min.y + textOffset),
                        event.style->getColorRole(ColorRole::Text));
                }
            }
        }
    }

    void TableWidget::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        FTK_P();
        if (p.hover.isValid())
        {
            p.hover = TableIndex();
            setDrawUpdate();
        }
    }

    void TableWidget::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        const TableIndex hover = _getCell(event.pos);
        if (hover != p.hover)
        {
            p.hover = hover;
            setDrawUpdate();
        }
    }

    void TableWidget::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        const TableIndex index = _getCell(event.pos);
        if (index.isValid())
        {
            takeKeyFocus();
            setCurrent(index);
            if (p.callback)
            {
                p.callback(index);
            }
        }
    }

    void TableWidget::keyFocusEvent(bool value)
    {
        IMouseWidget::keyFocusEvent(value);
        FTK_P();
        if (value && !p.current.isValid())
        {
            _move(0, 0);
        }
        setDrawUpdate();
    }

    void TableWidget::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (0 == event.modifiers && hasKeyFocus())
        {
            switch (event.key)
            {
            case Key::Return:
                if (p.current.isValid())
                {
                    event.accept = true;
                    if (p.callback)
                    {
                        p.callback(p.current);
                    }
                }
                break;
            case Key::Up:
                event.accept = true;
                _move(-1, 0);
                break;
            case Key::Down:
                event.accept = true;
                _move(1, 0);
                break;
            case Key::Left:
                event.accept = true;
                _move(0, -1);
                break;
            case Key::Right:
                event.accept = true;
                _move(0, 1);
                break;
            case Key::Home:
                event.accept = true;
                _move(-static_cast<int>(p.rows.size()), 0);
                break;
            case Key::End:
                event.accept = true;
                _move(static_cast<int>(p.rows.size()), 0);
                break;
            case Key::Escape:
                if (showKeyFocus())
                {
                    event.accept = true;
                    releaseKeyFocus();
                }
                break;
            default: break;
            }
        }
        if (!event.accept)
        {
            IMouseWidget::keyPressEvent(event);
        }
    }

    void TableWidget::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    bool TableWidget::_isEditable(const TableIndex& value) const
    {
        FTK_P();
        return
            value.row >= 0 &&
            value.row < static_cast<int>(p.rows.size()) &&
            value.column >= 0 &&
            value.column < static_cast<int>(p.rows[value.row].cells.size()) &&
            p.rows[value.row].visible &&
            p.rows[value.row].cells[value.column].editable;
    }

    TableIndex TableWidget::_getCell(const V2I& pos) const
    {
        FTK_P();
        TableIndex out;
        if (!p.geom.init)
        {
            for (size_t i = 0; i < p.rows.size() && !out.isValid(); ++i)
            {
                if (p.rows[i].visible &&
                    pos.y >= p.geom.rowY[i] &&
                    pos.y < p.geom.rowY[i] + p.size.rowHeight)
                {
                    for (size_t j = 0; j < p.rows[i].cells.size(); ++j)
                    {
                        if (pos.x >= p.geom.columnX[j] &&
                            pos.x < p.geom.columnX[j] + p.geom.columnW[j])
                        {
                            const TableIndex index(static_cast<int>(i), static_cast<int>(j));
                            if (_isEditable(index))
                            {
                                out = index;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
        return out;
    }

    void TableWidget::_move(int rows, int columns)
    {
        FTK_P();

        // The editable cells, in reading order.
        std::vector<TableIndex> cells;
        for (size_t i = 0; i < p.rows.size(); ++i)
        {
            for (size_t j = 0; j < p.rows[i].cells.size(); ++j)
            {
                const TableIndex index(static_cast<int>(i), static_cast<int>(j));
                if (_isEditable(index))
                {
                    cells.push_back(index);
                }
            }
        }
        if (cells.empty())
            return;

        TableIndex current = p.current;
        if (!current.isValid())
        {
            current = rows < 0 ? cells.back() : cells.front();
        }
        else if (columns != 0)
        {
            // Along the row.
            for (const auto& i : cells)
            {
                if (i.row == current.row &&
                    i.column == current.column + columns)
                {
                    current = i;
                    break;
                }
            }
        }
        else if (rows != 0)
        {
            // To the nearest editable cell of the column, as many rows
            // away as asked for or as there are.
            std::vector<TableIndex> column;
            for (const auto& i : cells)
            {
                if (i.column == current.column)
                {
                    column.push_back(i);
                }
            }
            int index = 0;
            for (size_t i = 0; i < column.size(); ++i)
            {
                if (column[i].row == current.row)
                {
                    index = static_cast<int>(i);
                }
            }
            index = std::max(0, std::min(index + rows, static_cast<int>(column.size()) - 1));
            current = column[index];
        }

        if (current != p.current)
        {
            p.current = current;
            setDrawUpdate();
        }
        _scrollToCurrent();
    }

    void TableWidget::_geomUpdate()
    {
        FTK_P();
        if (!p.geom.init || p.size.init)
            return;
        p.geom.init = false;

        const Box2I g = margin(getGeometry(), -p.size.margin);

        // The columns that stretch share the width the others leave,
        // equally if that fits what is in them, and otherwise each has
        // its own width and a share of what is spare.
        int stretch = 0;
        int width = 0;
        int stretchWidth = 0;
        int stretchMax = 0;
        for (int i = 0; i < p.columnCount; ++i)
        {
            width += p.size.columnWidths[i];
            if (getColumnStretch(i))
            {
                ++stretch;
                stretchWidth += p.size.columnWidths[i];
                stretchMax = std::max(stretchMax, p.size.columnWidths[i]);
            }
        }
        const int spare = std::max(0, g.w() - width);
        const bool equal = stretch > 0 && (stretchWidth + spare) / stretch >= stretchMax;
        p.geom.columnX.resize(p.columnCount);
        p.geom.columnW.resize(p.columnCount);
        int x = g.min.x;
        int count = 0;
        for (int i = 0; i < p.columnCount; ++i)
        {
            int w = p.size.columnWidths[i];
            if (stretch > 0 && getColumnStretch(i))
            {
                // Whole pixels, with the remainder going to the last.
                ++count;
                const int total = equal ? (stretchWidth + spare) : spare;
                const int share = count < stretch ?
                    total / stretch :
                    total - (total / stretch) * (stretch - 1);
                w = equal ? share : (w + share);
            }
            p.geom.columnX[i] = x;
            p.geom.columnW[i] = w;
            x += w;
        }

        p.geom.rowY.resize(p.rows.size());
        int y = g.min.y;
        bool first = true;
        for (size_t i = 0; i < p.rows.size(); ++i)
        {
            const auto& row = p.rows[i];
            if (row.visible && row.heading && !first)
            {
                y += p.size.headingSpace;
            }
            p.geom.rowY[i] = y;
            if (row.visible)
            {
                y += p.size.rowHeight;
                first = false;
            }
        }
    }

    void TableWidget::_editorUpdate()
    {
        FTK_P();
        if (p.editor)
        {
            // The editor covers its cell, and is centered over it if it
            // is the taller of the two.
            Box2I g = getCellRect(p.editorIndex);
            const int h = p.editor->getSizeHint().h;
            if (g.h() > 0 && h > g.h())
            {
                g = Box2I(g.min.x, g.min.y - (h - g.h()) / 2, g.w(), h);
            }
            p.editor->setGeometry(g);
        }
    }

    void TableWidget::_editorRemove()
    {
        FTK_P();
        if (p.editor)
        {
            const bool keyFocus = p.editor->containsKeyFocus();
            p.editor->setParent(nullptr);
            p.editor.reset();
            p.editorIndex = TableIndex();
            p.editorHadFocus = false;
            p.editorClose = false;
            if (keyFocus)
            {
                takeKeyFocus();
            }
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    void TableWidget::_scrollToCurrent()
    {
        FTK_P();
        // The scroll area is not the table's own: see ItemButtonList.
        if (auto scrollWidget = getParentT<ScrollWidget>())
        {
            if (const auto& content = scrollWidget->getWidget())
            {
                const Box2I g = getCellRect(p.current);
                if (g.w() > 0 && g.h() > 0)
                {
                    scrollWidget->scrollTo(Box2I(
                        g.min - content->getGeometry().min,
                        g.size()));
                }
            }
        }
    }
}
