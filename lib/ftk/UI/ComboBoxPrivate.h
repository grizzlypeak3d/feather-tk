// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ComboBox.h>
#include <ftk/UI/IButton.h>
#include <ftk/UI/IMenuPopup.h>

namespace ftk
{
    class ComboBoxButton : public IButton
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const ComboBoxItem&,
            const std::shared_ptr<IWidget>& parent);

        ComboBoxButton();

    public:
        virtual ~ComboBoxButton();

        static std::shared_ptr<ComboBoxButton> create(
            const std::shared_ptr<Context>&,
            const ComboBoxItem&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCurrent(bool);

        FTK_API Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    class ComboBoxMenu : public IMenuPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::vector<ComboBoxItem>&,
            int currentIndex,
            const std::shared_ptr<IWidget>& parent);

        ComboBoxMenu();

    public:
        virtual ~ComboBoxMenu();

        static std::shared_ptr<ComboBoxMenu> create(
            const std::shared_ptr<Context>&,
            const std::vector<ComboBoxItem>&,
            int currentIndex,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCallback(const std::function<void(int)>&);

        void setGeometry(const Box2I&) override;
        void keyFocusEvent(bool) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        void _setCurrent(int);
        void _currentUpdate();
        void _scrollToCurrent();

        FTK_PRIVATE();
    };
}
