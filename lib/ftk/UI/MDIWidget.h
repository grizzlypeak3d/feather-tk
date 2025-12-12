// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    class MDICanvas;

    //! \name MDI Widgets
    ///@{

    //! MDI resize directions.
    enum class FTK_API_TYPE MDIResize
    {
        None,
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest,

        Count,
        First = North
    };
    FTK_ENUM(MDIResize);

    //! MDI widget.
    class FTK_API_TYPE MDIWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::shared_ptr<IWidget>& parent);

        MDIWidget();

    public:
        FTK_API virtual ~MDIWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<MDIWidget> create(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the title.
        FTK_API const std::string& getTitle() const;

        //! Set the title.
        FTK_API void setTitle(const std::string&);

        //! Get the widget.
        FTK_API const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        FTK_API void setWidget(const std::shared_ptr<IWidget>&);

        //! Set the press callback.
        FTK_API void setPressCallback(const std::function<void(bool)>&);

        //! Set the move callback.
        FTK_API void setMoveCallback(const std::function<void(const V2I&)>&);

        //! Set the resize callback.
        FTK_API void setResizeCallback(const std::function<void(MDIResize, const V2I&)>&);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        Box2I _addMargins(const Box2I&) const;
        Box2I _removeMargins(const Box2I&) const;
        Size2I _removeMargins(const Size2I&) const;

        friend class MDICanvas;

        FTK_PRIVATE();
    };

    ///@}
}
