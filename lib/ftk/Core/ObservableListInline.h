// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <algorithm>

namespace ftk
{
    template<typename T>
    inline void ListObserver<T>::_init(
        const std::shared_ptr<IObservableList<T> >& value,
        const std::function<void(const std::vector<T>&)>& callback,
        ObserverAction action)
    {
        _value = value;
        _callback = callback;
        if (auto value = _value.lock())
        {
            value->_add(ListObserver<T>::shared_from_this());
            if (ObserverAction::Trigger == action)
            {
                _callback(value->get());
            }
        }
    }

    template<typename T>
    inline ListObserver<T>::~ListObserver()
    {
        if (auto value = _value.lock())
        {
            value->_removeExpired();
        }
    }

    template<typename T>
    inline std::shared_ptr<ListObserver<T> > ListObserver<T>::create(
        const std::shared_ptr<IObservableList<T> >& value,
        const std::function<void(const std::vector<T>&)>& callback,
        ObserverAction action)
    {
        std::shared_ptr<ListObserver<T> > out(new ListObserver<T>);
        out->_init(value, callback, action);
        return out;
    }

    template<typename T>
    inline void ListObserver<T>::doCallback(const std::vector<T>& value)
    {
        _callback(value);
    }

    template<typename T>
    inline IObservableList<T>::~IObservableList()
    {}

    template<typename T>
    inline size_t IObservableList<T>::getObserversCount() const
    {
        return _observers.size();
    }

    template<typename T>
    inline void IObservableList<T>::_add(const std::weak_ptr<ListObserver<T> >& observer)
    {
        _observers.push_back(observer);
    }

    template<typename T>
    inline void IObservableList<T>::_removeExpired()
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
    inline ObservableList<T>::ObservableList(const std::vector<T>& value) :
        _value(value)
    {}

    template<typename T>
    inline std::shared_ptr<ObservableList<T> > ObservableList<T>::create()
    {
        return std::shared_ptr<ObservableList<T> >(new ObservableList<T>);
    }

    template<typename T>
    inline std::shared_ptr<ObservableList<T> > ObservableList<T>::create(const std::vector<T>& value)
    {
        return std::shared_ptr<ObservableList<T> >(new ObservableList<T>(value));
    }

    template<typename T>
    inline void ObservableList<T>::setAlways(const std::vector<T>& value)
    {
        _value = value;
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline bool ObservableList<T>::setIfChanged(const std::vector<T>& value)
    {
        if (value == _value)
            return false;
        _value = value;
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
        return true;
    }

    template<typename T>
    inline void ObservableList<T>::clear()
    {
        if (_value.size())
        {
            _value.clear();
            for (const auto& i : IObservableList<T>::_observers)
            {
                if (auto observer = i.lock())
                {
                    observer->doCallback(_value);
                }
            }
        }
    }

    template<typename T>
    inline void ObservableList<T>::setItem(size_t index, const T& value)
    {
        _value[index] = value;
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline bool ObservableList<T>::setItemOnlyIfChanged(size_t index, const T& value)
    {
        if (value == _value[index])
            return false;
        _value[index] = value;
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
        return true;
    }

    template<typename T>
    inline void ObservableList<T>::pushBack(const T& value)
    {
        _value.push_back(value);
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline void ObservableList<T>::pushBack(const std::vector<T>& value)
    {
        _value.insert(_value.end(), value.begin(), value.end());
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline void ObservableList<T>::insertItem(size_t index, const T& value)
    {
        _value.insert(_value.begin() + index, value);
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline void ObservableList<T>::insertItems(size_t index, const std::vector<T>& value)
    {
        _value.insert(_value.begin() + index, value.begin(), value.end());
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline void ObservableList<T>::removeItem(size_t index)
    {
        _value.erase(_value.begin() + index);
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline void ObservableList<T>::removeItems(size_t start, size_t end)
    {
        _value.erase(_value.begin() + start, _value.begin() + end);
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline void ObservableList<T>::replaceItems(
        size_t start,
        size_t end,
        const std::vector<T>& items)
    {
        _value.erase(_value.begin() + start, _value.begin() + end);
        _value.insert(_value.begin() + start, items.begin(), items.end());
        for (const auto& i : IObservableList<T>::_observers)
        {
            if (auto observer = i.lock())
            {
                observer->doCallback(_value);
            }
        }
    }

    template<typename T>
    inline const std::vector<T>& ObservableList<T>::get() const
    {
        return _value;
    }

    template<typename T>
    inline size_t ObservableList<T>::getSize() const
    {
        return _value.size();
    }

    template<typename T>
    inline bool ObservableList<T>::isEmpty() const
    {
        return _value.empty();
    }

    template<typename T>
    inline T ObservableList<T>::getItem(size_t index) const
    {
        return _value[index];
    }

    template<typename T>
    inline bool ObservableList<T>::contains(const T& value) const
    {
        const auto i = std::find_if(
            _value.begin(),
            _value.end(),
            [value](const T& other)
            {
                return value == other;
            });
        return i != _value.end();
    }

    template<typename T>
    inline size_t ObservableList<T>::indexOf(const T& value) const
    {
        const auto i = std::find_if(
            _value.begin(),
            _value.end(),
            [value](const T& other)
            {
                return value == other;
            });
        return i != _value.end() ? i - _value.begin() : ObservableListInvalidIndex;
    }
}
