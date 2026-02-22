// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>

namespace widgets
{
    class App;
    class DragWidget;

    class DragDropData : public ftk::IDragDropData
    {
    public:
        DragDropData(const std::shared_ptr<DragWidget>&);

        virtual ~DragDropData();

        const std::shared_ptr<DragWidget>& getWidget() const;

    private:
        std::shared_ptr<DragWidget> _widget;
    };

    class DragWidget : public ftk::IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            int,
            const std::shared_ptr<ftk::IWidget>& parent);

        DragWidget() = default;

    public:
        virtual ~DragWidget();

        static std::shared_ptr<DragWidget> create(
            const std::shared_ptr<ftk::Context>&,
            int,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;
        void sizeHintEvent(const ftk::SizeHintEvent&) override;
        void drawEvent(const ftk::Box2I&, const ftk::DrawEvent&) override;
        void mouseEnterEvent(ftk::MouseEnterEvent&) override;
        void mouseLeaveEvent() override;
        void mouseMoveEvent(ftk::MouseMoveEvent&) override;

    private:
        std::shared_ptr<ftk::Label> _label;
        int _number = 0;
        int _dragLength = 0;
    };

    class ContainerWidget : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        ContainerWidget() = default;

    public:
        virtual ~ContainerWidget();

        static std::shared_ptr<ContainerWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void addWidget(const std::shared_ptr<DragWidget>&);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;
        void sizeHintEvent(const ftk::SizeHintEvent&) override;
        void drawOverlayEvent(const ftk::Box2I&, const ftk::DrawEvent&) override;
        void dragEnterEvent(ftk::DragDropEvent&) override;
        void dragLeaveEvent(ftk::DragDropEvent&) override;
        void dragMoveEvent(ftk::DragDropEvent&) override;
        void dropEvent(ftk::DragDropEvent&) override;

    private:
        int _getDropIndex(const ftk::V2I&) const;
        ftk::Box2I _getDropGeom(int) const;

        std::shared_ptr<ftk::VerticalLayout> _layout;
        int _handle = 0;
        int _dropTarget = -1;
    };

    class DragDrop : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        DragDrop() = default;

    public:
        virtual ~DragDrop();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::shared_ptr<ftk::HorizontalLayout> _layout;
    };
}
