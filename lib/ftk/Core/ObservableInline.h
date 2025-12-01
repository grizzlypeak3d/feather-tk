// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <algorithm>

namespace ftk
{
    template<typename T>
    inline void Observer<T>::_init(
        const std::shared_ptr<IObservable<T> >& value,
        const std::function<void(const T&)>& callback,
        ObserverAction action)
    {
        _value = value;
        _callback = callback;
        if (auto value = _value.lock())
        {
            value->_add(Observer<T>::shared_from_this());
            if (ObserverAction::Trigger == action)
            {
                _callback(value->get());
            }
        }
    }

    template<typename T>
    inline Observer<T>::~Observer()
    {
        if (auto value = _value.lock())
        {
            value->_removeExpired();
        }
    }

    template<typename T>
    inline std::shared_ptr<Observer<T> > Observer<T>::create(
        const std::shared_ptr<IObservable<T> >& value,
        const std::function<void(const T&)>& callback,
        ObserverAction action)
    {
        std::shared_ptr<Observer<T> > out(new Observer<T>);
        out->_init(value, callback, action);
        return out;
    }

    template<typename T>
    inline void Observer<T>::doCallback(const T& value)
    {
        _callback(value);
    }

    template<typename T>
    inline IObservable<T>::~IObservable()
    {}

    template<typename T>
    inline std::size_t IObservable<T>::getObserversCount() const
    {
        return _observers.size();
    }

    template<typename T>
    inline void IObservable<T>::_add(const std::weak_ptr<Observer<T> >& observer)
    {
        _observers.push_back(observer);
    }

    template<typename T>
    inline void IObservable<T>::_removeExpired()
    {
        auto i = _observers.begin();
        while (i != _observers.end())
        {
            if (i->expired())
            {
                i = _observers.erase(i);
            }
            else
            {
                ++i;
            }
        }
    }

    template<typename T>
    inline Observable<T>::Observable(const T& value) :
        _value(value)
    {}

    template<typename T>
    inline std::shared_ptr<Observable<T> > Observable<T>::create()
    {
        return std::shared_ptr<Observable<T> >(new Observable<T>);
    }

    template<typename T>
    inline std::shared_ptr<Observable<T> > Observable<T>::create(const T& value)
    {
        return std::shared_ptr<Observable<T> >(new Observable<T>(value));
    }

    template<typename T>
    inline void Observable<T>::setAlways(const T& value)
    {
        _value = value;
        for (const auto& i : IObservable<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline bool Observable<T>::setIfChanged(const T& value)
    {
        if (value == _value)
            return false;
        _value = value;
        for (const auto& i : IObservable<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
        return true;
    }

    template<typename T>
    inline const T& Observable<T>::get() const
    {
        return _value;
    }
}
