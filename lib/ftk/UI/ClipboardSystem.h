// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ISystem.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    //! Clipboard system.
    class ClipboardSystem : public ISystem
    {
    protected:
        ClipboardSystem(const std::shared_ptr<Context>&);

    public:
        virtual ~ClipboardSystem();

        //! Create a new system.
        static std::shared_ptr<ClipboardSystem> create(
            const std::shared_ptr<Context>&);

        //! Get the clipboard text.
        std::string getText() const;

        //! Set the clipboard text.
        void setText(const std::string&);

    private:
        FTK_PRIVATE();
    };

    ///@}
}