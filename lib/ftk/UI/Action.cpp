// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Action.h>

namespace ftk
{
    KeyShortcut::KeyShortcut(Key key, int modifiers) :
        key(key),
        modifiers(modifiers)
    {}

    bool KeyShortcut::operator == (const KeyShortcut& other) const
    {
        return
            key == other.key &&
            modifiers == other.modifiers;
    }

    bool KeyShortcut::operator != (const KeyShortcut& other) const
    {
        return !(*this == other);
    }

    struct Action::Private
    {
        std::shared_ptr<Observable<std::string> > text;
        std::shared_ptr<Observable<std::string> > icon;
        std::shared_ptr<Observable<std::string> > checkedIcon;
        std::shared_ptr<ObservableList<KeyShortcut> > shortcuts;
        std::function<void(void)> callback;
        std::shared_ptr<Observable<bool> > checkable;
        std::shared_ptr<Observable<bool> > checked;
        std::function<void(bool)> checkedCallback;
        std::shared_ptr<Observable<bool> > enabled;
        std::shared_ptr<Observable<std::string> > tooltip;
    };

    void Action::_init(
        const std::string& text,
        const std::string& icon,
        const KeyShortcut& shortcut,
        const std::function<void(void)>& callback,
        const std::function<void(bool)>& checkedCallback)
    {
        FTK_P();

        p.text = Observable<std::string>::create(text);
        p.icon = Observable<std::string>::create(icon);
        p.checkedIcon = Observable<std::string>::create();
        p.shortcuts = ObservableList<KeyShortcut>::create({ shortcut });
        p.callback = callback;
        p.checkable = Observable<bool>::create(bool(checkedCallback));
        p.checked = Observable<bool>::create(false);
        p.checkedCallback = checkedCallback;
        p.enabled = Observable<bool>::create(true);
        p.tooltip = Observable<std::string>::create();
    }

    Action::Action() :
        _p(new Private)
    {}

    Action::~Action()
    {}

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const std::function<void(void)>& callback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            std::string(),
            KeyShortcut(),
            callback,
            nullptr);
        return out;
    }

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const std::string& icon,
        const std::function<void(void)>& callback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            icon,
            KeyShortcut(),
            callback,
            nullptr);
        return out;
    }

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const KeyShortcut& shortcut,
        const std::function<void(void)>& callback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            std::string(),
            shortcut,
            callback,
            nullptr);
        return out;
    }

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const std::string& icon,
        const KeyShortcut& shortcut,
        const std::function<void(void)>& callback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            icon,
            shortcut,
            callback,
            nullptr);
        return out;
    }

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const std::function<void(bool)>& checkedCallback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            std::string(),
            KeyShortcut(),
            nullptr,
            checkedCallback);
        return out;
    }

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const std::string& icon,
        const std::function<void(bool)>& checkedCallback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            icon,
            KeyShortcut(),
            nullptr,
            checkedCallback);
        return out;
    }

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const KeyShortcut& shortcut,
        const std::function<void(bool)>& checkedCallback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            std::string(),
            shortcut,
            nullptr,
            checkedCallback);
        return out;
    }

    std::shared_ptr<Action> Action::create(
        const std::string& text,
        const std::string& icon,
        const KeyShortcut& shortcut,
        const std::function<void(bool)>& checkedCallback)
    {
        auto out = std::shared_ptr<Action>(new Action);
        out->_init(
            text,
            icon,
            shortcut,
            nullptr,
            checkedCallback);
        return out;
    }

    const std::string& Action::getText() const
    {
        return _p->text->get();
    }

    std::shared_ptr<IObservable<std::string> > Action::observeText() const
    {
        return _p->text;
    }

    void Action::setText(const std::string& value)
    {
        _p->text->setIfChanged(value);
    }

    const std::string& Action::getIcon() const
    {
        return _p->icon->get();
    }

    const std::string& Action::getCheckedIcon() const
    {
        return _p->checkedIcon->get();
    }

    std::shared_ptr<IObservable<std::string> > Action::observeIcon() const
    {
        return _p->icon;
    }

    std::shared_ptr<IObservable<std::string> > Action::observeCheckedIcon() const
    {
        return _p->checkedIcon;
    }

    void Action::setIcon(const std::string& value)
    {
        _p->icon->setIfChanged(value);
    }

    void Action::setCheckedIcon(const std::string& value)
    {
        _p->checkedIcon->setIfChanged(value);
    }

    const std::vector<KeyShortcut>& Action::getShortcuts() const
    {
        return _p->shortcuts->get();
    }

    std::shared_ptr<IObservableList<KeyShortcut> > Action::observeShortcuts() const
    {
        return _p->shortcuts;
    }

    void Action::setShortcuts(const std::vector<KeyShortcut>& value)
    {
        _p->shortcuts->setIfChanged(value);
    }

    void Action::doCallback()
    {
        if (_p->callback)
        {
            _p->callback();
        }
    }

    bool Action::isCheckable() const
    {
        return _p->checkable->get();
    }

    bool Action::isChecked() const
    {
        return _p->checked->get();
    }

    std::shared_ptr<IObservable<bool> > Action::observeCheckable() const
    {
        return _p->checkable;
    }

    std::shared_ptr<IObservable<bool> > Action::observeChecked() const
    {
        return _p->checked;
    }

    void Action::setCheckable(bool value)
    {
        FTK_P();
        p.checkable->setIfChanged(value);
    }

    void Action::setChecked(bool value)
    {
        FTK_P();
        p.checked->setIfChanged(value);
    }

    void Action::doCheckedCallback(bool value)
    {
        if (_p->checkedCallback)
        {
            _p->checkedCallback(value);
        }
    }

    bool Action::isEnabled() const
    {
        return _p->enabled->get();
    }

    std::shared_ptr<IObservable<bool> > Action::observeEnabled() const
    {
        return _p->enabled;
    }

    void Action::setEnabled(bool value)
    {
        FTK_P();
        p.enabled->setIfChanged(value);
    }

    const std::string& Action::getTooltip() const
    {
        return _p->tooltip->get();
    }

    std::shared_ptr<IObservable<std::string> > Action::observeTooltip() const
    {
        return _p->tooltip;
    }

    void Action::setTooltip(const std::string& value)
    {
        _p->tooltip->setIfChanged(value);
    }
}