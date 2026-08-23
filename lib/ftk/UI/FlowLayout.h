// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Flow layout.
    //!
    //! Children are placed left to right and wrap onto a new line when the
    //! next one will not fit. Use this where a horizontal layout would be
    //! right if only it always had the room -- a legend, a row of tags, a
    //! toolbar in a panel the user can make narrow.
    //!
    //! The height depends on the width, which the size hint has no way to ask
    //! about, so the layout reports the tallest single child until it has been
    //! given a geometry, then reports what that width actually needed. Laying
    //! one out therefore settles over two passes rather than one.
    class FTK_UI_API_TYPE FlowLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FlowLayout();

    public:
        FTK_UI_API virtual ~FlowLayout();

        //! Create a new layout.
        FTK_UI_API static std::shared_ptr<FlowLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_UI_API SizeRole getSpacingRole() const;

        //! Set the spacing role, used both between children on a line and
        //! between the lines.
        FTK_UI_API void setSpacingRole(SizeRole);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        //! Place the children within the given width, returning the height
        //! used. Passing a null geometry measures without moving anything,
        //! which is what the size hint needs.
        int _flow(const Box2I&, bool place);

        FTK_PRIVATE();
    };

    ///@}
}
