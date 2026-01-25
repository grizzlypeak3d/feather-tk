// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <algorithm>

namespace ftk
{
    template<typename T, typename U>
    inline void MapObserver<T, U>::_init(
        const std::shared_ptr<IObservableMap<T, U> >& value,
        const std::function<void(const std::map<T, U>&)>& callback,
        ObserverAction action)
    {
        _value = value;
        _callback = callback;
        if (auto value = _value.lock())
        {
            value->_add(MapObserver<T, U>::shared_from_this());
            if (ObserverAction::Trigger == action)
            {
                _callback(value->get());
            }
        }
    }

    template<typename T, typename U>
    inline MapObserver<T, U>::~MapObserver()
    {
        if (auto value = _value.lock())
        {
            value->_removeExpired();
        }
    }

    template<typename T, typename U>
    inline std::shared_ptr<MapObserver<T, U> > MapObserver<T, U>::create(
        const std::shared_ptr<IObservableMap<T, U> >& value,
        const std::function<void(const std::map<T, U>&)>& callback,
        ObserverAction action)
    {
        std::shared_ptr<MapObserver<T, U> > out(new MapObserver<T, U>);
        out->_init(value, callback, action);
        return out;
    }

    template<typename T, typename U>
    inline void MapObserver<T, U>::doCallback(const std::map<T, U>& value)
    {
        _callback(value);
    }

    template<typename T, typename U>
    inline IObservableMap<T, U>::~IObservableMap()
    {}

    template<typename T, typename U>
    inline std::size_t IObservableMap<T, U>::getObserversCount() const
    {
        return _observers.size();
    }

    template<typename T, typename U>
    inline void IObservableMap<T, U>::_add(const std::weak_ptr<MapObserver<T, U> >& observer)
    {
        _observers.push_back(observer);
    }

    template<typename T, typename U>
    inline void IObservableMap<T, U>::_removeExpired()
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

    template<typename T, typename U>
    inline ObservableMap<T, U>::ObservableMap(const std::map<T, U>& value) :
        _value(value)
    {}

    template<typename T, typename U>
    inline std::shared_ptr<ObservableMap<T, U> > ObservableMap<T, U>::create()
    {
        return std::shared_ptr<ObservableMap<T, U> >(new ObservableMap<T, U>);
    }

    template<typename T, typename U>
    inline std::shared_ptr<ObservableMap<T, U> > ObservableMap<T, U>::create(const std::map<T, U>& value)
    {
        return std::shared_ptr<ObservableMap<T, U> >(new ObservableMap<T, U>(value));
    }

    template<typename T, typename U>
    inline void ObservableMap<T, U>::setAlways(const std::map<T, U>& value)
    {
        _value = value;
        for (const auto& i : IObservableMap<T, U>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T, typename U>
    inline bool ObservableMap<T, U>::setIfChanged(const std::map<T, U>& value)
    {
        if (value == _value)
            return false;
        _value = value;
        for (const auto& i : IObservableMap<T, U>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
        return true;
    }

    template<typename T, typename U>
    inline void ObservableMap<T, U>::clear()
    {
        if (_value.size())
        {
            _value.clear();
            for (const auto& s : IObservableMap<T, U>::_observers)
            {
                if (auto observer = s.lock())
                {
                    observer->doCallback(_value);
                }
            }
        }
    }

    template<typename T, typename U>
    inline void ObservableMap<T, U>::setItem(const T& key, const U& value)
    {
        _value[key] = value;

        for (const auto& s : IObservableMap<T, U>::_observers)
        {
            if (auto observer = s.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T, typename U>
    inline void ObservableMap<T, U>::setItemOnlyIfChanged(const T& key, const U& value)
    {
        const auto i = _value.find(key);
        if (i != _value.end() && i->second == value)
            return;
        _value[key] = value;
        for (const auto& s : IObservableMap<T, U>::_observers)
        {
            if (auto observer = s.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T, typename U>
    inline const std::map<T, U>& ObservableMap<T, U>::get() const
    {
        return _value;
    }

    template<typename T, typename U>
    inline std::size_t ObservableMap<T, U>::getSize() const
    {
        return _value.size();
    }

    template<typename T, typename U>
    inline bool ObservableMap<T, U>::isEmpty() const
    {
        return _value.empty();
    }

    template<typename T, typename U>
    inline std::vector<T> ObservableMap<T, U>::getKeys() const
    {
        std::vector<T> out;
        for (const auto i : _value)
        {
            out.push_back(i.first);
        }
        return out;
    }

    template<typename T, typename U>
    inline bool ObservableMap<T, U>::hasKey(const T& value)
    {
        return _value.find(value) != _value.end();
    }

    template<typename T, typename U>
    inline const U& ObservableMap<T, U>::getItem(const T& key) const
    {
        return _value.find(key)->second;
    }
}
