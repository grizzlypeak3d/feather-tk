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
        FTK_API virtual ~TabBar();

        //! Create a new widget.
        FTK_API static std::shared_ptr<TabBar> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the tabs.
        FTK_API const std::vector<std::string>& getTabs() const;

        //! Set the tabs.
        FTK_API void setTabs(
            const std::vector<std::string>&,
            const std::vector<std::string>& tooltips = std::vector<std::string>());

        //! Add a tab.
        FTK_API void addTab(
            const std::string&,
            const std::string& tooltip = std::string());

        //! Remove a tab.
        FTK_API void removeTab(int);

        //! Clear the tabs.
        FTK_API void clear();

        //! Get the current tab.
        FTK_API int getCurrentTab() const;

        //! Set the current tab.
        FTK_API void setCurrentTab(int);

        //! Set the current tab callback.
        FTK_API void setCurrentTabCallback(const std::function<void(int)>&);

        //! Set the tab text.
        FTK_API void setTabText(int index, const std::string&);

        //! Set the tab tooltip.
        FTK_API void setTabTooltip(int index, const std::string&);

        //! Get whether the tabs are closable.
        FTK_API bool areTabsClosable() const;

        //! Set whether the tabs are closable.
        FTK_API void setTabsClosable(bool);

        //! Set the tab close callback.
        FTK_API void setTabCloseCallback(const std::function<void(int)>&);

        //! Get whether the scroll bar is visible.
        FTK_API bool isScrollBarVisible() const;

        //! Set whether the scroll bar is visible.
        FTK_API void setScrollBarVisible(bool);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void keyFocusEvent(bool) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _widgetUpdate();
        void _setCurrent(int);
        void _currentUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
