// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/IMenuPopup.h>

namespace ftk
{
    //! \name Menu Widgets
    ///@{

    //! Combo box menu.
    //!
    //! The popup list a combo box opens, for a widget that wants the same
    //! choice without the combo box around it -- a tool button that picks
    //! from a list, say.
    class FTK_UI_API_TYPE ComboBoxMenu : public IMenuPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::vector<ComboBoxItem>&,
            int currentIndex,
            const std::shared_ptr<IWidget>& parent);

        ComboBoxMenu();

    public:
        FTK_UI_API virtual ~ComboBoxMenu();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ComboBoxMenu> create(
            const std::shared_ptr<Context>&,
            const std::vector<ComboBoxItem>&,
            int currentIndex,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the callback, called with the index of the item chosen.
        FTK_UI_API void setCallback(const std::function<void(int)>&);

        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void keyFocusEvent(bool) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _setCurrent(int);
        void _currentUpdate();
        void _scrollToCurrent();

        FTK_PRIVATE();
    };

    ///@}
}
