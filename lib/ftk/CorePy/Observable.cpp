// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Observable.h>
#include <ftk/Core/Range.h>
#include <ftk/Core/Vector.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void observable(nb::module_& m)
        {
            nb::enum_<ObserverAction>(m, "ObserverAction")
                .value("Trigger", ObserverAction::Trigger)
                .value("Suppress", ObserverAction::Suppress);

            observable<int>(m, "Int");
            observable<float>(m, "Float");
            observable<double>(m, "Double");
            observable<bool>(m, "Bool");
            observable<std::string>(m, "String");
            observable<size_t>(m, "SizeT");
            observable<RangeI>(m, "RangeI");
            observable<RangeF>(m, "RangeF");
            observable<RangeD>(m, "RangeD");
            observable<V2I>(m, "V2I");
        }
    }
}
