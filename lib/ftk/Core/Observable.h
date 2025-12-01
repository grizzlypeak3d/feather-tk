// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Util.h>

#include <functional>
#include <memory>
#include <vector>

namespace ftk
{
    //! \name Observables
    ///@{
        
    //! Observer actions.
    enum class ObserverAction
    {
        Trigger,
        Suppress
    };

    template<typename T>
    class IObservable;

    //! Value observer.
    template<typename T>
    class Observer : public std::enable_shared_from_this<Observer<T> >
    {
        FTK_NON_COPYABLE(Observer);

    protected:
        void _init(
            const std::shared_ptr<IObservable<T> >&,
            const std::function<void(const T&)>&,
            ObserverAction);

        Observer() = default;

    public:
        ~Observer();

        //! Create a new observer.
        static std::shared_ptr<Observer<T> > create(
            const std::shared_ptr<IObservable<T> >&,
            const std::function<void(const T&)>&,
            ObserverAction = ObserverAction::Trigger);

        //! Execute the callback.
        void doCallback(const T&);

    private:
        std::function<void(const T&)> _callback;
        std::weak_ptr<IObservable<T> > _value;
    };

    //! Base class for observable values.
    template<typename T>
    class IObservable : public std::enable_shared_from_this<IObservable<T> >
    {
    public:
        virtual ~IObservable() = 0;

        //! Get the value.
        virtual const T& get() const = 0;

        //! Get the number of observers.
        std::size_t getObserversCount() const;

    protected:
        void _add(const std::weak_ptr<Observer<T> >&);
        void _removeExpired();

        std::vector<std::weak_ptr<Observer<T> > > _observers;

        friend class Observer<T>;
    };

    //! Observable value.
    template<typename T>
    class Observable : public IObservable<T>
    {
        FTK_NON_COPYABLE(Observable);

    protected:
        Observable() = default;
        explicit Observable(const T&);

    public:
        //! Create a new observable.
        static std::shared_ptr<Observable<T> > create();

        //! Create a new observable.
        static std::shared_ptr<Observable<T> > create(const T&);

        //! Set the value.
        void setAlways(const T&);

        //! Set the value only if it has changed.
        bool setIfChanged(const T&);

        const T& get() const override;

    private:
        T _value = T();
    };

    typedef Observer<int> IntObserver;
    typedef Observable<int> IntObservable;
        
    ///@}
}

#include <ftk/Core/ObservableInline.h>