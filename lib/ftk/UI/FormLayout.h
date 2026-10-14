// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/IWidget.h>

#include <memory>
#include <vector>

namespace ftk
{
    //! \name Layouts
    ///@{

    class FormLayout;

    //! A group of form layouts whose labels share one column width, so
    //! that forms in separate sections of a panel line up as one would.
    //!
    //! The width is that of the widest visible label in any of the forms,
    //! and is known when the first of them is sized: a form asks the group,
    //! which measures the labels of all of them. A form whose section is
    //! closed still counts, so opening a section does not move the others.
    class FTK_UI_API_TYPE FormGroup : public std::enable_shared_from_this<FormGroup>
    {
        FTK_NON_COPYABLE(FormGroup);

    protected:
        FormGroup();

    public:
        FTK_UI_API ~FormGroup();

        //! Create a new group.
        FTK_UI_API static std::shared_ptr<FormGroup> create();

    private:
        friend class FormLayout;

        int _getLabelWidth(const SizeHintEvent&);

        std::vector<std::weak_ptr<FormLayout> > _forms;
    };

        //! Form layout.
    class FTK_UI_API_TYPE FormLayout : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FormLayout();

    public:
        virtual ~FormLayout();

        //! Create a new layout.
        FTK_UI_API static std::shared_ptr<FormLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add a row.
        FTK_UI_API int addRow(const std::string&, const std::shared_ptr<IWidget>&);

        //! Remove a row.
        FTK_UI_API void removeRow(int);

        //! Remove a row.
        FTK_UI_API void removeRow(const std::shared_ptr<IWidget>&);

        //! Clear all of the rows.
        FTK_UI_API void clear();

        //! Set the text.
        FTK_UI_API void setText(int, const std::string&);

        //! Set the text.
        FTK_UI_API void setText(const std::shared_ptr<IWidget>&, const std::string&);

        //! Set row visibility.
        FTK_UI_API void setRowVisible(int, bool);

        //! Set row visibility.
        FTK_UI_API void setRowVisible(const std::shared_ptr<IWidget>&, bool);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_UI_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_UI_API void setSpacingRole(SizeRole);

        //! Add a spacer.
        FTK_UI_API int addSpacer();

        //! Add a spacer.
        FTK_UI_API int addSpacer(SizeRole);

        //! Get the group the layout's labels share their width with.
        FTK_UI_API const std::shared_ptr<FormGroup>& getGroup() const;

        //! Set a group for the layout's labels to share their width with,
        //! or none.
        FTK_UI_API void setGroup(const std::shared_ptr<FormGroup>&);

        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;


    private:
        friend class FormGroup;

        int _getLabelWidth(const SizeHintEvent&);

        FTK_PRIVATE();
    };

    //! Put every form layout among a widget and its descendants into a
    //! group: for a panel assembled from widgets that each hold a form of
    //! their own. Forms added to them afterwards are not included.
    FTK_UI_API void setFormGroup(
        const std::shared_ptr<IWidget>&,
        const std::shared_ptr<FormGroup>&);

    ///@}
}
