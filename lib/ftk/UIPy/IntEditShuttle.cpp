// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntEditShuttle.h>

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
        void intEditShuttle(nb::module_& m)
        {
            nb::class_<IntEditShuttle, IContainer>(m, "IntEditShuttle")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&IntEditShuttle::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&IntEditShuttle::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &IntEditShuttle::getValue, &IntEditShuttle::setValue)
                .def("setCallback", &IntEditShuttle::setCallback)
                .def("setPressedCallback", &IntEditShuttle::setPressedCallback)
                .def_prop_rw("range", &IntEditShuttle::getRange, nb::overload_cast<const RangeI&>(&IntEditShuttle::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<int, int>(&IntEditShuttle::setRange))
                .def_prop_rw("step", &IntEditShuttle::getStep, &IntEditShuttle::setStep)
                .def_prop_rw("largeStep", &IntEditShuttle::getLargeStep, &IntEditShuttle::setLargeStep)
                .def_prop_rw("defaultValue", &IntEditShuttle::getDefault, &IntEditShuttle::setDefault)
                .def("getModel", &IntEditShuttle::getModel)
                .def_prop_rw("font", &IntEditShuttle::getFont, &IntEditShuttle::setFont);
        }
    }
}
