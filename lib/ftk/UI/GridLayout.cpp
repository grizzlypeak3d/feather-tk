// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/GridLayout.h>

#include <ftk/UI/LayoutUtil.h>

#include <optional>

namespace ftk
{
    namespace
    {
        struct GridPos
        {
            int row = 0;
            int column = 0;
        };
    }

    struct GridLayout::Private
    {
        std::map<std::shared_ptr<IWidget>, GridPos> gridPos;
        ColorRole rowBackgroundRole = ColorRole::None;
        SizeRole marginRole = SizeRole::None;
        SizeRole hSpacingRole = SizeRole::Spacing;
        SizeRole vSpacingRole = SizeRole::Spacing;

        struct SizeData
        {
            bool init = true;
            int margin = 0;
            int hSpacing = 0;
            int vSpacing = 0;
        };
        SizeData size;

        struct GeomData
        {
            std::vector<bool> rowsVisible;
            std::vector<int> rowSizes;
        };
        GeomData geom;

        GridPos getSize() const;

        void getSizeHints(
            std::vector<int>& rows,
            std::vector<int>& columns) const;

        void getStretch(
            std::vector<bool>& rows,
            std::vector<bool>& columns) const;

        void getVisible(
            int& rows,
            int& columns) const;

        void getVisible(
            std::vector<bool>& rows,
            std::vector<bool>& columns) const;
    };

    void GridLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::GridLayout", parent);
    }

    GridLayout::GridLayout() :
        _p(new Private)
    {}

    GridLayout::~GridLayout()
    {}

    std::shared_ptr<GridLayout> GridLayout::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<GridLayout>(new GridLayout);
        out->_init(context, parent);
        return out;
    }

    std::pair<int, int> GridLayout::getGridPos(const std::shared_ptr<IWidget>& widget) const
    {
        FTK_P();
        std::pair<int, int> out(-1, -1);
        const auto i = p.gridPos.find(widget);
        if (i != p.gridPos.end())
        {
            out.first = i->second.row;
            out.second = i->second.column;
        }
        return out;
    }

    int GridLayout::getGridRow(const std::shared_ptr<IWidget>& widget) const
    {
        FTK_P();
        int out = -1;
        const auto i = p.gridPos.find(widget);
        if (i != p.gridPos.end())
        {
            out = i->second.row;
        }
        return out;
    }

    int GridLayout::getGridColumn(const std::shared_ptr<IWidget>& widget) const
    {
        FTK_P();
        int out = -1;
        const auto i = p.gridPos.find(widget);
        if (i != p.gridPos.end())
        {
            out = i->second.column;
        }
        return out;
    }

    void GridLayout::setGridPos(
        const std::shared_ptr<IWidget>& child,
        int row,
        int column)
    {
        FTK_P();
        p.gridPos[child].row = row;
        p.gridPos[child].column = column;
        setSizeUpdate();
        setDrawUpdate();
    }

    ColorRole GridLayout::getRowBackgroundRole() const
    {
        return _p->rowBackgroundRole;
    }

    void GridLayout::setRowBackgroundRole(ColorRole value)
    {
        FTK_P();
        if (value == p.rowBackgroundRole)
            return;
        p.rowBackgroundRole = value;
        setDrawUpdate();
    }

    SizeRole GridLayout::getMarginRole() const
    {
        return _p->marginRole;
    }

    void GridLayout::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole GridLayout::getSpacingRole() const
    {
        return _p->hSpacingRole;
    }

    void GridLayout::setSpacingRole(SizeRole value)
    {
        setSpacingRole(value, value);
    }

    void GridLayout::setSpacingRole(SizeRole horizontal, SizeRole vertical)
    {
        FTK_P();
        if (horizontal == p.hSpacingRole && vertical == p.vSpacingRole)
            return;
        p.hSpacingRole = horizontal;
        p.vSpacingRole = vertical;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    void GridLayout::clear()
    {
        auto children = getChildren();
        for (auto child : children)
        {
            child->setParent(nullptr);
        }
    }
    
    Size2I GridLayout::getSizeHint() const
    {
        FTK_P();
        Size2I out;

        // Get size hints.
        std::vector<int> rowSizeHints;
        std::vector<int> columnSizeHints;
        p.getSizeHints(rowSizeHints, columnSizeHints);
        for (int i : rowSizeHints)
        {
            out.h += i;
        }
        for (int i : columnSizeHints)
        {
            out.w += i;
        }

        // Add spacing.
        int rowsVisibleCount = 0;
        int columnsVisibleCount = 0;
        p.getVisible(rowsVisibleCount, columnsVisibleCount);
        if (rowsVisibleCount > 0)
        {
            out.h += (rowsVisibleCount - 1) * p.size.vSpacing;
        }
        if (columnsVisibleCount > 0)
        {
            out.w += (columnsVisibleCount - 1) * p.size.hSpacing;
        }

        // Add the margin.
        out.w += p.size.margin * 2;
        out.h += p.size.margin * 2;

        return out;
    }

    void GridLayout::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();

        // Get the total size.
        V2I totalSize;
        std::vector<int> rowSizeHints;
        std::vector<int> columnSizeHints;
        p.getSizeHints(rowSizeHints, columnSizeHints);
        for (const auto& i : rowSizeHints)
        {
            totalSize.y += i;
        }
        for (const auto& i : columnSizeHints)
        {
            totalSize.x += i;
        }

        // Get the visible rows and columns.
        std::vector<bool> columnsVisible;
        p.getVisible(p.geom.rowsVisible, columnsVisible);

        // Get the layout stretch.
        std::vector<bool> rowStretch;
        std::vector<bool> columnStretch;
        p.getStretch(rowStretch, columnStretch);

        // Get the layout stretch size.
        const Box2I g = margin(getGeometry(), -p.size.margin);
        size_t rowStretchCount = 0;
        size_t columnStretchCount = 0;
        for (bool i : rowStretch)
        {
            if (i)
            {
                ++rowStretchCount;
            }
        }
        for (bool i : columnStretch)
        {
            if (i)
            {
                ++columnStretchCount;
            }
        }
        V2I stretchSize;
        V2I stretchExtra;
        int rowsVisibleCount = 0;
        int columnsVisibleCount = 0;
        p.getVisible(rowsVisibleCount, columnsVisibleCount);
        if (rowStretchCount > 0)
        {
            const int avail = g.h() -
                (rowsVisibleCount - 1) * p.size.vSpacing -
                totalSize.y;
            stretchSize.y = avail / static_cast<int>(rowStretchCount);
            stretchExtra.y = avail - stretchSize.y * static_cast<int>(rowStretchCount);
        }
        if (columnStretchCount > 0)
        {
            const int avail = g.w() -
                (columnsVisibleCount - 1) * p.size.hSpacing -
                totalSize.x;
            stretchSize.x = avail / static_cast<int>(columnStretchCount);
            stretchExtra.x = avail - stretchSize.x * static_cast<int>(columnStretchCount);
        }

        // Find the last stretch row and column, which absorb the remainder
        // left over from the integer division above so the grid fills the
        // available space exactly instead of coming up a pixel or two short.
        int lastRowStretch = -1;
        for (int i = 0; i < static_cast<int>(rowStretch.size()); ++i)
        {
            if (rowStretch[i])
            {
                lastRowStretch = i;
            }
        }
        int lastColumnStretch = -1;
        for (int i = 0; i < static_cast<int>(columnStretch.size()); ++i)
        {
            if (columnStretch[i])
            {
                lastColumnStretch = i;
            }
        }

        // Get the sizes.
        p.geom.rowSizes.clear();
        std::vector<int> columnSizes;
        for (int i = 0; i < static_cast<int>(rowSizeHints.size()); ++i)
        {
            int size = rowSizeHints[i];
            if (rowStretch[i])
            {
                size += stretchSize.y;
                if (i == lastRowStretch)
                {
                    size += stretchExtra.y;
                }
            }
            p.geom.rowSizes.push_back(size);
        }
        for (int i = 0; i < static_cast<int>(columnSizeHints.size()); ++i)
        {
            int size = columnSizeHints[i];
            if (columnStretch[i])
            {
                size += stretchSize.x;
                if (i == lastColumnStretch)
                {
                    size += stretchExtra.x;
                }
            }
            columnSizes.push_back(size);
        }

        // Layout the children.
        for (const auto& i : p.gridPos)
        {
            const bool visible = i.first->isVisible(false);
            V2I pos = g.min;
            for (int j = 0; j < i.second.row && j < static_cast<int>(p.geom.rowSizes.size()); ++j)
            {
                if (p.geom.rowsVisible[j])
                {
                    pos.y += p.geom.rowSizes[j] + (visible ? p.size.vSpacing : 0);
                }
            }
            for (int j = 0; j < i.second.column && j < static_cast<int>(columnSizes.size()); ++j)
            {
                if (columnsVisible[j])
                {
                    pos.x += columnSizes[j] + (visible ? p.size.hSpacing : 0);
                }
            }
            Size2I size;
            if (i.second.column < static_cast<int>(columnSizes.size()))
            {
                size.w = columnSizes[i.second.column];
            }
            if (i.second.row < static_cast<int>(p.geom.rowSizes.size()))
            {
                size.h = p.geom.rowSizes[i.second.row];
            }
            i.first->setGeometry(
                align(
                    Box2I(pos, size),
                    i.first->getSizeHint(),
                    i.first->getHAlign(),
                    i.first->getVAlign()));
        }
    }

    void GridLayout::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
        }
    }

    void GridLayout::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
            p.size.hSpacing = event.style->getSizeRole(p.hSpacingRole, event.displayScale);
            p.size.vSpacing = event.style->getSizeRole(p.vSpacingRole, event.displayScale);
        }
    }

    void GridLayout::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();
        if (p.rowBackgroundRole != ColorRole::None)
        {
            const Box2I g = margin(getGeometry(), -p.size.margin);
            int y = g.min.y;
            int row = 0;
            for (size_t i = 0;
                i < p.geom.rowSizes.size() && i < p.geom.rowsVisible.size();
                ++i)
            {
                if (p.geom.rowsVisible[i])
                {
                    if (row % 2 == 1)
                    {
                        event.render->drawRect(
                            Box2I(g.min.x, y, g.w(), p.geom.rowSizes[i]),
                            event.style->getColorRole(p.rowBackgroundRole));
                    }
                    y += p.geom.rowSizes[i] + p.size.vSpacing;
                    ++row;
                }
            }
        }
    }

    void GridLayout::childRemoveEvent(const ChildRemoveEvent& event)
    {
        FTK_P();
        const auto i = p.gridPos.find(event.child);
        if (i != p.gridPos.end())
        {
            p.gridPos.erase(i);
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    GridPos GridLayout::Private::getSize() const
    {
        GridPos out;
        for (const auto& i : gridPos)
        {
            out.row = std::max(out.row, i.second.row);
            out.column = std::max(out.column, i.second.column);
        }
        if (!gridPos.empty())
        {
            out.row += 1;
            out.column += 1;
        }
        return out;
    }

    void GridLayout::Private::getSizeHints(
        std::vector<int>& rows,
        std::vector<int>& columns) const
    {
        const GridPos size = getSize();
        rows = std::vector<int>(size.row, 0);
        columns = std::vector<int>(size.column, 0);
        for (const auto& i : gridPos)
        {
            const bool visible = i.first->isVisible(false);
            if (visible)
            {
                const Size2I& sizeHint = i.first->getSizeHint();
                rows[i.second.row] = std::max(
                    rows[i.second.row],
                    sizeHint.h);
                columns[i.second.column] = std::max(
                    columns[i.second.column],
                    sizeHint.w);
            }
        }
    }

    void GridLayout::Private::getStretch(
        std::vector<bool>& rows,
        std::vector<bool>& columns) const
    {
        const GridPos size = getSize();
        rows = std::vector<bool>(size.row, false);
        columns = std::vector<bool>(size.column, false);
        for (const auto& i : gridPos)
        {
            const bool visible = i.first->isVisible(false);
            if (visible)
            {
                if (Stretch::Expanding == i.first->getVStretch())
                {
                    rows[i.second.row] = true;
                }
                if (Stretch::Expanding == i.first->getHStretch())
                {
                    columns[i.second.column] = true;
                }
            }
        }
    }

    void GridLayout::Private::getVisible(
        int& rows,
        int& columns) const
    {
        rows = 0;
        columns = 0;
        std::vector<bool> rowsVisible;
        std::vector<bool> columnsVisible;
        getVisible(rowsVisible, columnsVisible);
        for (bool i : rowsVisible)
        {
            if (i)
            {
                ++rows;
            }
        }
        for (bool i : columnsVisible)
        {
            if (i)
            {
                ++columns;
            }
        }
    }

    void GridLayout::Private::getVisible(
        std::vector<bool>& rows,
        std::vector<bool>& columns) const
    {
        const GridPos size = getSize();
        rows = std::vector<bool>(size.row, false);
        columns = std::vector<bool>(size.column, false);
        for (const auto& i : gridPos)
        {
            const bool visible = i.first->isVisible(false);
            rows[i.second.row] = rows[i.second.row] || visible;
            columns[i.second.column] = columns[i.second.column] || visible;
        }
    }
}