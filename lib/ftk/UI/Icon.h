// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Icons
    ///@{
        
    //! Icon widget.
    class FTK_API_TYPE Icon : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Icon();

    public:
        virtual ~Icon();

        //! Create a new widget.
        static std::shared_ptr<Icon> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<Icon> create(
            const std::shared_ptr<Context>&,
            const std::string& icon,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the icon.
        const std::string& getIcon() const;

        //! Set the icon.
        void setIcon(const std::string&);

        //! Get the margin role.
        SizeRole getMarginRole() const;

        //! Set the margin role.
        void setMarginRole(SizeRole);

        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
