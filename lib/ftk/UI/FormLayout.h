// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Form layout.
    class FTK_API_TYPE FormLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FormLayout();

    public:
        virtual ~FormLayout();

        //! Create a new layout.
        FTK_API static std::shared_ptr<FormLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add a row.
        FTK_API int addRow(const std::string&, const std::shared_ptr<IWidget>&);

        //! Remove a row.
        FTK_API void removeRow(int);

        //! Remove a row.
        FTK_API void removeRow(const std::shared_ptr<IWidget>&);

        //! Clear all of the rows.
        FTK_API void clear();

        //! Set row visibility.
        FTK_API void setRowVisible(int, bool);

        //! Set row visibility.
        FTK_API void setRowVisible(const std::shared_ptr<IWidget>&, bool);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_API void setSpacingRole(SizeRole);

        //! Add a spacer.
        FTK_API int addSpacer();

        //! Add a spacer.
        FTK_API int addSpacer(SizeRole);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
