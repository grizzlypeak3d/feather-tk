// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/MDIWidget.h>

namespace ftk
{
    //! \name MDI Widgets
    ///@{

    //! MDI canvas.
    //!
    //! \todo Add support for maximizing MDI widgets.
    class FTK_API_TYPE MDICanvas : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        MDICanvas();

    public:
        FTK_API virtual ~MDICanvas();

        //! Create a new canvas.
        FTK_API static std::shared_ptr<MDICanvas> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the canvas size.
        FTK_API const Size2I& getCanvasSize() const;

        //! Set the canvas size.
        FTK_API void setCanvasSize(const Size2I&);

        //! Get the grid size.
        FTK_API const Size2I& getGridSize() const;

        //! Set the grid size.
        FTK_API void setGridSize(const Size2I&);

        //! Add a widget to the canvas.
        FTK_API std::shared_ptr<MDIWidget> addWidget(
            const std::string& title,
            const V2I& pos,
            const std::shared_ptr<IWidget>&);

        //! Set the child geometry callback.
        void setChildGeometryCallback(
            const std::function<void(const std::vector<Box2I>&)>&);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void childRemoveEvent(const ChildRemoveEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        int _snapToGridX(int) const;
        int _snapToGridY(int) const;
        Size2I _snapToGrid(const Size2I&) const;

        FTK_PRIVATE();
    };

    ///@}
}
