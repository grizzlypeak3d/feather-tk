// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ISystem.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    //! Clipboard system.
    class FTK_API_TYPE ClipboardSystem : public ISystem
    {
    protected:
        ClipboardSystem(const std::shared_ptr<Context>&);

    public:
        FTK_API virtual ~ClipboardSystem();

        //! Create a new system.
        FTK_API static std::shared_ptr<ClipboardSystem> create(
            const std::shared_ptr<Context>&);

        //! Get the clipboard text.
        FTK_API std::string getText() const;

        //! Set the clipboard text.
        FTK_API void setText(const std::string&);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
