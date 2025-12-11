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
        virtual ~Splitter();

        //! Create a new widget.
        static std::shared_ptr<Splitter> create(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the split value.
        float getSplit() const;

        //! Set the split value.
        void setSplit(float);

        //! Get whether the splitter has a border.
        bool hasBorder() const;

        //! Set whether the splitter has a border.
        void setBorder(bool);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mouseEnterEvent(MouseEnterEvent&) override;
        void mouseLeaveEvent() override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
