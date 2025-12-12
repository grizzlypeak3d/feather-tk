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
        FTK_API virtual ~ProgressDialog();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ProgressDialog> create(
            const std::shared_ptr<Context>& context,
            const std::string& title,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        FTK_API const std::string& getText() const;

        //! Set the text.
        FTK_API void setText(const std::string&);

        //! Get the progress message.
        FTK_API const std::string& getMessage() const;

        //! Set the progress message.
        FTK_API void setMessage(const std::string&);

        //! Get the range.
        FTK_API const RangeD& getRange() const;

        //! Set the range.
        FTK_API void setRange(const RangeD&);

        //! Set the range.
        FTK_API void setRange(double, double);

        //! Get the value.
        FTK_API double getValue() const;

        //! Set the value.
        FTK_API void setValue(double);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
