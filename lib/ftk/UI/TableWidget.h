// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Table Widgets
    ///@{

    //! Table cell.
    struct FTK_UI_API_TYPE TableCell
    {
        TableCell() = default;
        FTK_UI_API explicit TableCell(
            const std::string& text,
            bool editable = false,
            ColorRole colorRole = ColorRole::None);

        std::string text;

        //! An editable cell highlights under the mouse, can be made
        //! current, and calls the table's callback when it is clicked.
        bool editable = false;

        //! Background color role, drawn as a tint.
        ColorRole colorRole = ColorRole::None;

        bool operator == (const TableCell&) const = default;
    };

    //! Table row.
    struct FTK_UI_API_TYPE TableRow
    {
        TableRow() = default;
        FTK_UI_API explicit TableRow(
            const std::vector<TableCell>&,
            bool heading = false);

        std::vector<TableCell> cells;

        //! A heading row is drawn in bold with space above it, and over a
        //! band the width of the table, with a line beneath it, or both.
        bool heading = false;

        bool visible = true;

        bool operator == (const TableRow&) const = default;
    };

    //! Table index.
    struct FTK_UI_API_TYPE TableIndex
    {
        TableIndex() = default;
        FTK_UI_API TableIndex(int row, int column);

        int row = -1;
        int column = -1;

        FTK_UI_API bool isValid() const;

        bool operator == (const TableIndex&) const = default;
    };

    //! Table widget.
    //!
    //! The table draws its cells as text. To edit a cell, give the table an
    //! editor widget from the callback: it is placed over the cell until it
    //! is closed or the key focus leaves it.
    class FTK_UI_API_TYPE TableWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        TableWidget();

    public:
        FTK_UI_API virtual ~TableWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<TableWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Rows and Columns
        ///@{

        //! Get the rows.
        FTK_UI_API const std::vector<TableRow>& getRows() const;

        //! Set the rows.
        FTK_UI_API void setRows(const std::vector<TableRow>&);

        //! Get the number of columns.
        FTK_UI_API int getColumnCount() const;

        //! Get whether a column takes a share of the spare width.
        FTK_UI_API bool getColumnStretch(int) const;

        //! Set whether a column takes a share of the spare width. Columns
        //! are otherwise as wide as their widest cell.
        FTK_UI_API void setColumnStretch(int, bool);

        ///@}

        //! \name Current Cell
        ///@{

        //! Get the current cell.
        FTK_UI_API const TableIndex& getCurrent() const;

        //! Set the current cell. Only an editable cell can be current.
        FTK_UI_API void setCurrent(const TableIndex&);

        //! Set the callback for when an editable cell is clicked, or the
        //! return key is pressed on the current cell.
        FTK_UI_API void setCallback(const std::function<void(const TableIndex&)>&);

        //! Get a cell rectangle.
        FTK_UI_API Box2I getCellRect(const TableIndex&) const;

        ///@}

        //! \name Editor
        ///@{

        //! Get the editor widget.
        FTK_UI_API const std::shared_ptr<IWidget>& getEditor() const;

        //! Get the cell that is being edited.
        FTK_UI_API const TableIndex& getEditorIndex() const;

        //! Open an editor widget over a cell. The editor should take the
        //! key focus; it is closed when the key focus leaves it, or when
        //! it leaves a press of the return or escape key unaccepted.
        FTK_UI_API void openEditor(const TableIndex&, const std::shared_ptr<IWidget>&);

        //! Close the editor widget. The editor is removed on the next
        //! tick, so this can be called from the editor's own callbacks.
        FTK_UI_API void closeEditor();

        ///@}

        //! \name Options
        ///@{

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        //! Get whether lines are drawn between the columns.
        FTK_UI_API bool hasColumnLines() const;

        //! Set whether lines are drawn between the columns.
        FTK_UI_API void setColumnLines(bool);

        //! Get the color role of the heading rows.
        FTK_UI_API ColorRole getHeadingRole() const;

        //! Set the color role of the heading rows. With no color role,
        //! and nothing to edit in the first column, the text of the
        //! first column starts at the edge of the table.
        FTK_UI_API void setHeadingRole(ColorRole);

        //! Get whether a line is drawn beneath the heading rows.
        FTK_UI_API bool hasHeadingLine() const;

        //! Set whether a line is drawn beneath the heading rows.
        FTK_UI_API void setHeadingLine(bool);

        ///@}

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void tickEvent(
            bool parentsVisible,
            bool parentsEnabled,
            const TickEvent&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mouseLeaveEvent() override;
        FTK_UI_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void keyFocusEvent(bool) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        bool _isEditable(const TableIndex&) const;
        TableIndex _getCell(const V2I&) const;
        void _move(int rows, int columns);
        void _geomUpdate();
        void _editorUpdate();
        void _editorRemove();
        void _scrollToCurrent();

        FTK_PRIVATE();
    };

    ///@}
}
