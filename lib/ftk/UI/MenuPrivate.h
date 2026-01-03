// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Menu.h>

#include <ftk/UI/IButton.h>

namespace ftk
{
    class MenuButton : public IButton
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<Action>&,
            const std::shared_ptr<IWidget>& parent);

        MenuButton();

    public:
        virtual ~MenuButton();

        static std::shared_ptr<MenuButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<Action>& = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCurrent(bool);
        void setShortcuts(const std::vector<KeyShortcut>&);
        void setSubMenuIcon(const std::string&);

        void setEnabledCallback(const std::function<void(bool)>&);

        void setText(const std::string&) override;

        void setEnabled(bool) override;
        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(
            const Box2I&,
            const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
}