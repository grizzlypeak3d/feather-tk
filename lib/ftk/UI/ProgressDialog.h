// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IDialog.h>

#include <ftk/Core/Range.h>

namespace ftk
{
    //! \name Dialogs
    ///@{

    //! Progress dialog.
    class FTK_API_TYPE ProgressDialog : public IDialog
    {
    protected:
        void _init(
            const std::shared_ptr<Context>& context,
            const std::string& title,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent);

        ProgressDialog();

    public:
        virtual ~ProgressDialog();

        //! Create a new widget.
        static std::shared_ptr<ProgressDialog> create(
            const std::shared_ptr<Context>& context,
            const std::string& title,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        const std::string& getText() const;

        //! Set the text.
        void setText(const std::string&);

        //! Get the progress message.
        const std::string& getMessage() const;

        //! Set the progress message.
        void setMessage(const std::string&);

        //! Get the range.
        const RangeD& getRange() const;

        //! Set the range.
        void setRange(const RangeD&);

        //! Set the range.
        void setRange(double, double);

        //! Get the value.
        double getValue() const;

        //! Set the value.
        void setValue(double);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
