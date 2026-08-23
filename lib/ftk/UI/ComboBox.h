// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IMouseWidget.h>

#include <ftk/Core/String.h>

namespace ftk
{
    //! \name Menu Widgets
    ///@{
        
    //! Combo box item.
    struct FTK_UI_API_TYPE ComboBoxItem
    {
        ComboBoxItem() = default;
        FTK_UI_API explicit ComboBoxItem(
            const std::string& text,
            const std::string& icon = std::string());

        std::string text;
        std::string icon;

        FTK_UI_API bool operator == (const ComboBoxItem&) const;
        FTK_UI_API bool operator != (const ComboBoxItem&) const;
    };

    //! Combo box.
    class FTK_UI_API_TYPE ComboBox : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ComboBox();

    public:
        FTK_UI_API virtual ~ComboBox();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ComboBox> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ComboBox> create(
            const std::shared_ptr<Context>&,
            const std::vector<ComboBoxItem>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ComboBox> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the items.
        FTK_UI_API const std::vector<ComboBoxItem>& getItems() const;

        //! Set the items.
        FTK_UI_API void setItems(const std::vector<ComboBoxItem>&);

        //! Set the items.
        FTK_UI_API void setItems(const std::vector<std::string>&);

        //! Get the current index.
        FTK_UI_API int getCurrentIndex() const;

        //! Set the current index.
        FTK_UI_API void setCurrentIndex(int);

        //! Set the current index callback.
        FTK_UI_API void setIndexCallback(const std::function<void(int)>&);

        //! Set the current item callback.
        FTK_UI_API void setItemCallback(const std::function<void(const ComboBoxItem&)>&);

        //! Get the number of characters shown when the box is closed.
        FTK_UI_API size_t getElide() const;

        //! Get which end of an elided item is kept.
        FTK_UI_API ElideMode getElideMode() const;

        //! Set the number of characters shown when the box is closed, and
        //! which end of a longer item to keep. Zero, the default, shows the
        //! whole of the longest item, which is what makes the box as wide as
        //! its longest item however short the current one is. The menu is
        //! never elided, so the whole of each item can still be read when
        //! choosing.
        FTK_UI_API void setElide(size_t, ElideMode = ElideMode::Right);

        //! Get the font.
        FTK_UI_API FontType getFont() const;

        //! Set the font.
        FTK_UI_API void setFont(FontType);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_UI_API void mouseLeaveEvent() override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        ComboBoxItem _getItem(int) const;

        void _click();
        void _commit(int);

        FTK_PRIVATE();
    };
        
    ///@}
}
