// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Icons
    ///@{
        
    //! Icon widget.
    class FTK_UI_API_TYPE Icon : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Icon();

    public:
        FTK_UI_API virtual ~Icon();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<Icon> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<Icon> create(
            const std::shared_ptr<Context>&,
            const std::string& icon,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the icon.
        FTK_UI_API const std::string& getIcon() const;

        //! Set the icon.
        FTK_UI_API void setIcon(const std::string&);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
