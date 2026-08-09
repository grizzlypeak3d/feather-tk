// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Base class for widgets that are one child filling them.
    //!
    //! Most composite widgets are a layout, or a scroll area, or a tab widget,
    //! given the whole of the parent. Written by hand that is the same two
    //! methods every time:
    //!
    //! \code
    //! Size2I X::getSizeHint() const { return _layout->getSizeHint(); }
    //! void X::setGeometry(const Box2I& v)
    //! {
    //!     IWidget::setGeometry(v);
    //!     _layout->setGeometry(v);
    //! }
    //! \endcode
    //!
    //! Deriving from this and calling _setWidget() instead says the same thing
    //! once. Widgets that lay their children out themselves derive from IWidget
    //! as before; this is only for the ones that hand everything to one child.
    class FTK_API_TYPE IContainer : public IWidget
    {
        FTK_NON_COPYABLE(IContainer);

    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent);

        IContainer();

    public:
        FTK_API virtual ~IContainer() = 0;

        //! Get the child filling this widget.
        FTK_API const std::shared_ptr<IWidget>& getWidget() const;

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    protected:
        //! Set the child filling this widget. Replacing it detaches the one
        //! before, so the caller does not have to remember that a parent owns
        //! its children.
        FTK_API void _setWidget(const std::shared_ptr<IWidget>&);

    private:
        std::shared_ptr<IWidget> _widget;
    };

    ///@}
}
