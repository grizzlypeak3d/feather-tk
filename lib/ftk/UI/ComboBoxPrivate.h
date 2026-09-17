// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/ComboBoxMenu.h>
#include <ftk/UI/IButton.h>

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

        FTK_UI_API static std::shared_ptr<ComboBoxButton> create(
            const std::shared_ptr<Context>&,
            const ComboBoxItem&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCurrent(bool);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;
        void styleEvent(const StyleEvent&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        void _sizeDirty() override;

    private:
        FTK_PRIVATE();
    };
}
