// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Event.h>

namespace ftk
{
    //! \name Actions
    ///@{

    //! Action.
    class FTK_API_TYPE Action : public std::enable_shared_from_this<Action>
    {
    protected:
        void _init(
            const std::string&               text,
            const std::string&               icon,
            Key                              shortcut,
            int                              shortcutModifiers,
            const std::function<void(void)>& callback,
            const std::function<void(bool)>& checkedCallback);

        Action();

    public:
        FTK_API ~Action();

        //! \name Create
        ///@{

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::function<void(void)>& callback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const std::function<void(void)>& callback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            Key                              shortcut,
            int                              shortcutModifiers,
            const std::function<void(void)>& callback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            Key                              shortcut,
            int                              shortcutModifiers,
            const std::function<void(void)>& callback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::function<void(bool)>& checkedCallback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const std::function<void(bool)>& checkedCallback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            Key                              shortcut,
            int                              shortcutModifiers,
            const std::function<void(bool)>& checkedCallback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            Key                              shortcut,
            int                              shortcutModifiers,
            const std::function<void(bool)>& checkedCallback);

        ///@}

        //! \name Text
        ///@{

        FTK_API const std::string& getText() const;
        FTK_API std::shared_ptr<IObservable<std::string> > observeText() const;
        FTK_API void setText(const std::string&);

        ///@}

        //! \name Icon
        ///@{

        FTK_API const std::string& getIcon() const;
        FTK_API const std::string& getCheckedIcon() const;
        FTK_API std::shared_ptr<IObservable<std::string> > observeIcon() const;
        FTK_API std::shared_ptr<IObservable<std::string> > observeCheckedIcon() const;
        FTK_API void setIcon(const std::string&);
        FTK_API void setCheckedIcon(const std::string&);

        ///@}

        //! \name Shortcut
        ///@{

        FTK_API Key getShortcut() const;
        FTK_API int getShortcutModifiers() const;
        FTK_API std::shared_ptr<IObservable<Key> > observeShortcut() const;
        FTK_API std::shared_ptr<IObservable<int> > observeShortcutModifiers() const;
        FTK_API void setShortcut(Key);
        FTK_API void setShortcutModifiers(int);

        ///@}

        //! \name Callback
        ///@{

        FTK_API void doCallback();

        ///@}

        //! \name Checkable
        ///@{

        FTK_API bool isCheckable() const;
        FTK_API bool isChecked() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeCheckable() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeChecked() const;
        FTK_API void setCheckable(bool);
        FTK_API void setChecked(bool);
        FTK_API void doCheckedCallback(bool);

        ///@}

        //! \name Enabled
        ///@{

        FTK_API bool isEnabled() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeEnabled() const;
        FTK_API void setEnabled(bool);

        ///@}

        //! \name Tooltip
        ///@{

        FTK_API const std::string& getTooltip() const;
        FTK_API std::shared_ptr<IObservable<std::string> > observeTooltip() const;
        FTK_API void setTooltip(const std::string&);

        ///@}

    private:
        FTK_PRIVATE();
    };

    ///@}
}
