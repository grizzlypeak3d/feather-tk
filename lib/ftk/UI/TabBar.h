// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Tab bar widget.
    //! 
    //! \todo Add scrolling for the tab buttons.
    class FTK_API_TYPE TabBar : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        TabBar();

    public:
        virtual ~TabBar();

        //! Create a new widget.
        static std::shared_ptr<TabBar> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the tabs.
        const std::vector<std::string>& getTabs() const;

        //! Set the tabs.
        void setTabs(
            const std::vector<std::string>&,
            const std::vector<std::string>& tooltips = std::vector<std::string>());

        //! Add a tab.
        void addTab(
            const std::string&,
            const std::string& tooltip = std::string());

        //! Remove a tab.
        void removeTab(int);

        //! Clear the tabs.
        void clearTabs();

        //! Get the current tab.
        int getCurrentTab() const;

        //! Set the current tab.
        void setCurrentTab(int);

        //! Set the callback.
        void setCallback(const std::function<void(int)>&);

        //! Set the tab text.
        void setTabText(int index, const std::string&);

        //! Set the tab tooltip.
        void setTabTooltip(int index, const std::string&);

        //! Get whether the tabs are closable.
        bool areTabsClosable() const;

        //! Set whether the tabs are closable.
        void setTabsClosable(bool);

        //! Set the close callback.
        void setTabCloseCallback(const std::function<void(int)>&);

        //! Get whether the scroll bar is visible.
        bool isScrollBarVisible() const;

        //! Set whether the scroll bar is visible.
        void setScrollBarVisible(bool);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void keyFocusEvent(bool) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        void _widgetUpdate();
        void _setCurrent(int);
        void _currentUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
