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

    //! Splitter widget.
    class FTK_API_TYPE Splitter : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent);

        Splitter();

    public:
        FTK_API virtual ~Splitter();

        //! Create a new widget.
        FTK_API static std::shared_ptr<Splitter> create(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the widgets, replacing whatever is there.
        //!
        //! Building a tree of splitters by parenting each child one at a time
        //! means taking the old one apart in the right order first, since a
        //! parent owns its children: an old splitter whose only other reference
        //! is dropped stays in the tree and goes on drawing. This does the
        //! whole thing at once and detaches what it replaces.
        FTK_API void setWidgets(const std::vector<std::shared_ptr<IWidget> >&);

        //! Get the split value.
        FTK_API float getSplit() const;

        //! Set the split value.
        FTK_API void setSplit(float);

        //! Set the callback for the split being dragged.
        //!
        //! What a linked arrangement is built out of -- a four-up whose rows
        //! divide together needs each splitter to hear about the other.
        FTK_API void setSplitCallback(const std::function<void(float)>&);

        //! Get whether the splitter has a border.
        FTK_API bool hasBorder() const;

        //! Set whether the splitter has a border.
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
        //! Where the split falls along the given size, in pixels, kept far
        //! enough from either end for the whole handle to fit inside.
        int _split(int size) const;

        FTK_PRIVATE();
    };

    ///@}
}
