// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Tab widget.
    class FTK_API_TYPE TabWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        TabWidget();

    public:
        FTK_API virtual ~TabWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<TabWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the tabs.
        FTK_API const std::vector<std::string>& getTabs() const;

        //! Add a tab.
        FTK_API void addTab(
            const std::string& name,
            const std::shared_ptr<IWidget>& widget,
            const std::string& tooltip = std::string());

        //! Remove a tab.
        FTK_API void removeTab(int);

        //! Remove a tab.
        FTK_API void removeTab(const std::shared_ptr<IWidget>&);

        //! Clear the tabs.
        FTK_API void clearTabs();

        //! Get the current tab.
        FTK_API int getCurrentTab() const;

        //! Set the current tab.
        FTK_API void setCurrentTab(int);

        //! Set the current tab callback.
        FTK_API void setCurrentTabCallback(const std::function<void(int)>&);

        //! Get the current widget.
        FTK_API std::shared_ptr<IWidget> getCurrentWidget() const;

        //! Set the current widget.
        FTK_API void setCurrentWidget(const std::shared_ptr<IWidget>&);

        //! Set the current widget callback.
        FTK_API void setCurrentWidgetCallback(const std::function<void(const std::shared_ptr<IWidget>&)>&);

        //! Set the tab text.
        FTK_API void setTabText(int index, const std::string&);

        //! Set the tab text.
        FTK_API void setTabText(const std::shared_ptr<IWidget>&, const std::string&);

        //! Set the tab tooltip.
        FTK_API void setTabTooltip(int index, const std::string&);

        //! Set the tab tooltip.
        FTK_API void setTabTooltip(const std::shared_ptr<IWidget>&, const std::string&);

        //! Get whether the tabs are closable.
        FTK_API bool areTabsClosable() const;

        //! Set whether the tabs are closable.
        FTK_API void setTabsClosable(bool);

        //! Set the close callback.
        FTK_API void setTabCloseCallback(const std::function<void(int)>&);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
