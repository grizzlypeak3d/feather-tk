// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

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

        //! Get the split value.
        FTK_API float getSplit() const;

        //! Set the split value.
        FTK_API void setSplit(float);

        //! Get whether the splitter has a border.
        FTK_API bool hasBorder() const;

        //! Set whether the splitter has a border.
        FTK_API void setBorder(bool);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
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
