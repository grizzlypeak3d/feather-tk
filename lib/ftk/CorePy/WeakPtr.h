// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

//! \file
//! The type caster for std::weak_ptr, which nanobind does not carry one
//! for: an API that hands out a weak pointer -- DocumentModel::observeAdd()
//! and the like -- is otherwise a C++ type Python makes nothing of.
//!
//! Going out, the pointer is locked: Python is given the object, or None
//! where it has already gone. Coming in there is nothing to do it with. A
//! weak_ptr made from a Python object would have to be held by something,
//! and a Python object handed to C++ is held by the caller alone: the weak
//! pointer would expire the moment the call returned, which is worse than
//! refusing it. So an argument of this type is not accepted, and a binding
//! that needs one takes the shared pointer instead.

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>

#include <memory>

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

template <typename T>
struct type_caster<std::weak_ptr<T>>
{
    using SharedCaster = make_caster<std::shared_ptr<T>>;

    NB_TYPE_CASTER(std::weak_ptr<T>, SharedCaster::Name + const_name(" | None"))

    bool from_python(handle, uint32_t, cleanup_list*) noexcept
    {
        // See above: nothing would keep the object alive.
        return false;
    }

    static handle from_cpp(const std::weak_ptr<T>& value, rv_policy policy,
                           cleanup_list* cleanup) noexcept
    {
        std::shared_ptr<T> ptr = value.lock();
        if (!ptr)
        {
            return none().release();
        }
        return SharedCaster::from_cpp(std::move(ptr), policy, cleanup);
    }
};

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)
