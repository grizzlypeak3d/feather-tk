// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <functional>
#include <vector>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! A splitter that divides both ways at once, into four.
    //!
    //! Two splitters ganged together can hold four widgets, but they are still
    //! two splitters: the rows divide independently unless something keeps them
    //! in step, and the point where the divisions cross belongs to neither of
    //! them. Here there is one division each way and one place they cross, and
    //! dragging that crossing moves both -- which is the thing a four-up wants
    //! and the thing two splitters cannot offer.
    class FTK_API_TYPE Splitter2D : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Splitter2D();

    public:
        FTK_API virtual ~Splitter2D();

        //! Create a new widget.
        FTK_API static std::shared_ptr<Splitter2D> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the widgets, replacing whatever is there.
        //!
        //! In reading order: top left, top right, bottom left, bottom right.
        //! Fewer than four fills as many quadrants as there are and leaves the
        //! rest empty; the divisions do not move to suit.
        FTK_API void setWidgets(const std::vector<std::shared_ptr<IWidget> >&);

        //! Get where the divisions are, each from zero to one.
        FTK_API const V2F& getSplit() const;

        //! Set where the divisions are.
        FTK_API void setSplit(const V2F&);

        //! Set the callback for a division being dragged.
        FTK_API void setSplitCallback(const std::function<void(const V2F&)>&);

        //! Get whether the divisions have a border.
        FTK_API bool hasBorder() const;

        //! Set whether the divisions have a border.
        FTK_API void setBorder(bool);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void styleEvent(const StyleEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
