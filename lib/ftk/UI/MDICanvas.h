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
        virtual ~MDICanvas();

        //! Create a new canvas.
        static std::shared_ptr<MDICanvas> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the canvas size.
        const Size2I& getCanvasSize() const;

        //! Set the canvas size.
        void setCanvasSize(const Size2I&);

        //! Get the grid size.
        const Size2I& getGridSize() const;

        //! Set the grid size.
        void setGridSize(const Size2I&);

        //! Add a widget to the canvas.
        std::shared_ptr<MDIWidget> addWidget(
            const std::string& title,
            const V2I& pos,
            const std::shared_ptr<IWidget>&);

        void setGeometry(const Box2I&) override;
        void childRemoveEvent(const ChildRemoveEvent&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        int _snapToGridX(int) const;
        int _snapToGridY(int) const;
        Size2I _snapToGrid(const Size2I&) const;

        FTK_PRIVATE();
    };

    ///@}
}
