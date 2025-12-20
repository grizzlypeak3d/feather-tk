// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Event.h>

namespace ftk
{
    //! \name Actions
    ///@{

    //! Keyboard shortcut.
    struct FTK_API_TYPE KeyShortcut
    {
        KeyShortcut() = default;
        FTK_API KeyShortcut(Key, int modifiers = 0);

        Key key       = Key::Unknown;
        int modifiers = 0;

        FTK_API bool operator == (const KeyShortcut&) const;
        FTK_API bool operator != (const KeyShortcut&) const;
    };

    //! Action.
    class FTK_API_TYPE Action : public std::enable_shared_from_this<Action>
    {
    protected:
        void _init(
            const std::string&               text,
            const std::string&               icon,
            const KeyShortcut&               shortcut,
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
            const KeyShortcut&               shortcut,
            const std::function<void(void)>& callback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const KeyShortcut&               shortcut,
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
            const KeyShortcut&               shortcut,
            const std::function<void(bool)>& checkedCallback);

        FTK_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const KeyShortcut&               shortcut,
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

        FTK_API const std::vector<KeyShortcut>& getShortcuts() const;
        FTK_API std::shared_ptr<IObservableList<KeyShortcut> > observeShortcuts() const;
        FTK_API void setShortcuts(const std::vector<KeyShortcut>&);

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

    FTK_API void to_json(nlohmann::json&, const KeyShortcut&);

    FTK_API void from_json(const nlohmann::json&, KeyShortcut&);

    ///@}
}
