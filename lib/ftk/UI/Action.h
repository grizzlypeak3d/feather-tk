// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/Event.h>

namespace ftk
{
    //! \name Actions
    ///@{

    //! Keyboard shortcut.
    struct FTK_UI_API_TYPE KeyShortcut
    {
        KeyShortcut() = default;
        FTK_UI_API KeyShortcut(Key, KeyModifier);
        FTK_UI_API KeyShortcut(Key, KeyModifier, KeyModifier);
        FTK_UI_API KeyShortcut(Key, KeyModifier, KeyModifier, KeyModifier);
        FTK_UI_API KeyShortcut(Key, int modifiers = 0);

        Key key       = Key::Unknown;
        int modifiers = 0;

        FTK_UI_API bool operator == (const KeyShortcut&) const;
        FTK_UI_API bool operator != (const KeyShortcut&) const;
    };

    //! How an action carries a checked state.
    enum class FTK_UI_API_TYPE ActionCheckType
    {
        //! A command. Picking it does something and nothing is remembered.
        None,

        //! A switch. Picking it turns something on or off.
        Check,

        //! One of several. Picking it selects; picking the one already
        //! selected leaves it selected, because there is no such thing as
        //! none of them.
        //!
        //! Set by ActionGroup rather than by hand: an action is one of many
        //! only in the company of the others.
        Radio,

        Count,
        First = None
    };
    FTK_ENUM(ActionCheckType);

    //! Action.
    class FTK_UI_API_TYPE Action : public std::enable_shared_from_this<Action>
    {
        FTK_NON_COPYABLE(Action);

    protected:
        void _init(
            const std::string&               text,
            const std::string&               icon,
            const KeyShortcut&               shortcut,
            const std::function<void(void)>& callback,
            const std::function<void(bool)>& checkedCallback);

        Action();

    public:
        FTK_UI_API ~Action();

        //! \name Create
        ///@{

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::function<void(void)>& callback);

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const std::function<void(void)>& callback);

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const KeyShortcut&               shortcut,
            const std::function<void(void)>& callback);

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const KeyShortcut&               shortcut,
            const std::function<void(void)>& callback);

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::function<void(bool)>& checkedCallback);

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const std::function<void(bool)>& checkedCallback);

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const KeyShortcut&               shortcut,
            const std::function<void(bool)>& checkedCallback);

        FTK_UI_API static std::shared_ptr<Action> create(
            const std::string&               text,
            const std::string&               icon,
            const KeyShortcut&               shortcut,
            const std::function<void(bool)>& checkedCallback);

        ///@}

        //! \name Text
        ///@{

        FTK_UI_API const std::string& getText() const;
        FTK_UI_API std::shared_ptr<IObservable<std::string> > observeText() const;
        FTK_UI_API void setText(const std::string&);

        ///@}

        //! \name Icon
        ///@{

        FTK_UI_API const std::string& getIcon() const;
        FTK_UI_API const std::string& getCheckedIcon() const;
        FTK_UI_API std::shared_ptr<IObservable<std::string> > observeIcon() const;
        FTK_UI_API std::shared_ptr<IObservable<std::string> > observeCheckedIcon() const;
        FTK_UI_API void setIcon(const std::string&);
        FTK_UI_API void setCheckedIcon(const std::string&);

        ///@}

        //! \name Shortcut
        ///@{

        FTK_UI_API const std::vector<KeyShortcut>& getShortcuts() const;
        FTK_UI_API std::shared_ptr<IObservableList<KeyShortcut> > observeShortcuts() const;
        FTK_UI_API void setShortcuts(const std::vector<KeyShortcut>&);

        ///@}

        //! \name Callback
        ///@{

        FTK_UI_API void doCallback();

        ///@}

        //! \name Checkable
        ///@{

        FTK_UI_API ActionCheckType getCheckType() const;
        FTK_UI_API std::shared_ptr<IObservable<ActionCheckType> > observeCheckType() const;
        FTK_UI_API void setCheckType(ActionCheckType);

        //! Whether the action carries a checked state at all.
        FTK_UI_API bool isCheckable() const;

        FTK_UI_API bool isChecked() const;
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeChecked() const;
        FTK_UI_API void setChecked(bool);
        FTK_UI_API void doCheckedCallback(bool);

        ///@}

        //! \name Enabled
        ///@{

        FTK_UI_API bool isEnabled() const;
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeEnabled() const;
        FTK_UI_API void setEnabled(bool);

        ///@}

        //! \name Tooltip
        ///@{

        FTK_UI_API const std::string& getTooltip() const;
        FTK_UI_API std::shared_ptr<IObservable<std::string> > observeTooltip() const;
        FTK_UI_API void setTooltip(const std::string&);

        ///@}

    private:
        FTK_PRIVATE();
    };

    FTK_UI_API std::string to_string(const KeyShortcut&);

    FTK_UI_API bool from_string(const std::string&, KeyShortcut&);

    FTK_UI_API void to_json(nlohmann::json&, const KeyShortcut&);

    FTK_UI_API void from_json(const nlohmann::json&, KeyShortcut&);

    ///@}
}
