// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

//! \file
//! The std::function caster for the bindings, used in place of nanobind's
//! <nanobind/stl/function.h> (the two can't be included together).
//!
//! A Python callable handed to C++ -- a widget callback, an observer --
//! is held in a std::function that Python's garbage collector can't see
//! into. A callback that refers back to the object holding it (a menu
//! action whose lambda uses the script's globals, where the menu is one of
//! those globals) makes a cycle the collector can never break, and
//! everything in it, down to players with running threads, lives until
//! the interpreter is torn down. That crashes at exit, some of the time.
//!
//! This caster keeps a list of the callables it holds, and releases all of
//! them when Python runs its atexit handlers, while the interpreter is
//! still whole: the cycles are broken and their objects are destroyed in
//! order. A callback called after that does nothing.

#include <nanobind/nanobind.h>

#include <functional>
#include <mutex>
#include <type_traits>
#include <vector>

namespace ftk
{
    namespace python
    {
        //! The Python callables held by std::function objects.
        class FunctionRegistry
        {
        public:
            struct Node
            {
                PyObject* f = nullptr;
                Node* prev = nullptr;
                Node* next = nullptr;
            };

            static FunctionRegistry& get()
            {
                static FunctionRegistry* registry = new FunctionRegistry;
                return *registry;
            }

            std::mutex mutex;

            //! Add a node; the mutex must be held.
            void add(Node* node)
            {
                node->prev = nullptr;
                node->next = _head;
                if (_head)
                {
                    _head->prev = node;
                }
                _head = node;
            }

            //! Remove a node; the mutex must be held.
            void remove(Node* node)
            {
                if (node->prev)
                {
                    node->prev->next = node->next;
                }
                else
                {
                    _head = node->next;
                }
                if (node->next)
                {
                    node->next->prev = node->prev;
                }
                node->prev = nullptr;
                node->next = nullptr;
            }

            //! Release every callable. Called with the GIL held.
            void release()
            {
                std::vector<PyObject*> fs;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    for (Node* node = _head; node; node = node->next)
                    {
                        if (node->f)
                        {
                            fs.push_back(node->f);
                            node->f = nullptr;
                        }
                    }
                }
                // Outside the lock: releasing a callable can destroy
                // objects that hold more of them.
                for (PyObject* f : fs)
                {
                    Py_DECREF(f);
                }
            }

            //! Release the callables at exit; called the first time one
            //! is held, with the GIL held.
            void registerAtExit()
            {
                if (_atExit)
                    return;
                _atExit = true;
                try
                {
                    nanobind::module_::import_("atexit").attr("register")(
                        nanobind::cpp_function([] { get().release(); }));
                }
                catch (const std::exception&)
                {
                    // Only the clean exit is lost.
                    PyErr_Clear();
                }
            }

        private:
            Node* _head = nullptr;
            bool _atExit = false;
        };
    }
}

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

struct ftk_pyfunc_wrapper
{
    ftk::python::FunctionRegistry::Node node;

    explicit ftk_pyfunc_wrapper(PyObject* f)
    {
        auto& registry = ftk::python::FunctionRegistry::get();
        registry.registerAtExit();
        Py_INCREF(f);
        std::lock_guard<std::mutex> lock(registry.mutex);
        node.f = f;
        registry.add(&node);
    }

    ftk_pyfunc_wrapper(ftk_pyfunc_wrapper&& w) noexcept
    {
        auto& registry = ftk::python::FunctionRegistry::get();
        std::lock_guard<std::mutex> lock(registry.mutex);
        node.f = w.node.f;
        w.node.f = nullptr;
        registry.add(&node);
    }

    ftk_pyfunc_wrapper(const ftk_pyfunc_wrapper& w)
    {
        auto& registry = ftk::python::FunctionRegistry::get();
        std::lock_guard<std::mutex> lock(registry.mutex);
        node.f = w.node.f;
        if (node.f)
        {
            // Don't touch the reference count if the interpreter is shut
            // down.
            if (cleanup_guard guard{})
                Py_INCREF(node.f);
            else
                node.f = nullptr;
        }
        registry.add(&node);
    }

    ~ftk_pyfunc_wrapper()
    {
        auto& registry = ftk::python::FunctionRegistry::get();
        PyObject* f = nullptr;
        {
            std::lock_guard<std::mutex> lock(registry.mutex);
            f = node.f;
            node.f = nullptr;
            registry.remove(&node);
        }
        if (f)
        {
            // Don't run the deleter if the interpreter has been shut down.
            if (cleanup_guard guard{})
                Py_DECREF(f);
        }
    }

    //! A new reference to the callable, or null once it's released.
    PyObject* acquire() const
    {
        auto& registry = ftk::python::FunctionRegistry::get();
        std::lock_guard<std::mutex> lock(registry.mutex);
        Py_XINCREF(node.f);
        return node.f;
    }

    ftk_pyfunc_wrapper& operator=(const ftk_pyfunc_wrapper&) = delete;
    ftk_pyfunc_wrapper& operator=(ftk_pyfunc_wrapper&&) = delete;
};

template <typename Return, typename... Args>
struct type_caster<std::function<Return(Args...)>>
{
    using ReturnCaster = make_caster<
        std::conditional_t<std::is_void_v<Return>, void_type, Return>>;

    NB_TYPE_CASTER(std::function <Return(Args...)>,
                   const_name("collections.abc.Callable[[") +
                       concat(make_caster<Args>::Name...) + const_name("], ") +
                       ReturnCaster::Name + const_name("]"))

    struct pyfunc_wrapper_t : ftk_pyfunc_wrapper
    {
        using ftk_pyfunc_wrapper::ftk_pyfunc_wrapper;

        Return operator()(Args... args) const
        {
            gil_scoped_acquire acq;
            if (!acq.is_valid())
                raise("nanobind: cannot invoke a Python callable, the "
                      "interpreter is shutting down!");
            object f = steal(acquire());
            if (!f.is_valid())
            {
                // Released at exit.
                if constexpr (std::is_void_v<Return>)
                    return;
                else if constexpr (std::is_default_constructible_v<Return>)
                    return Return();
                else
                    raise("feather-tk: cannot invoke a Python callable, it "
                          "was released at exit");
            }
            return cast<Return>(f((forward_t<Args>) args...));
        }
    };

    bool from_python(handle src, uint32_t flags, cleanup_list*) noexcept
    {
        if (src.is_none())
            return flags & cast_flags::convert;

        if (!PyCallable_Check(src.ptr()))
            return false;

        value = pyfunc_wrapper_t(src.ptr());

        return true;
    }

    static handle from_cpp(const Value& value, rv_policy rvp,
                           cleanup_list*) noexcept
    {
        const pyfunc_wrapper_t* wrapper = value.template target<pyfunc_wrapper_t>();
        if (wrapper)
        {
            if (PyObject* f = wrapper->acquire())
                return handle(f);
            return none().release();
        }

        if (rvp == rv_policy::none)
            return handle();

        if (!value)
            return none().release();

        return cpp_function(value).release();
    }
};

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)
