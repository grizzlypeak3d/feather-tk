// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleEditShuttle.h>

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
        void doubleEditShuttle(nb::module_& m)
        {
            nb::class_<DoubleEditShuttle, IContainer>(m, "DoubleEditShuttle")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEditShuttle::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<DoubleModel>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEditShuttle::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &DoubleEditShuttle::getValue, &DoubleEditShuttle::setValue)
                .def("setCallback", &DoubleEditShuttle::setCallback)
                .def("setPressedCallback", &DoubleEditShuttle::setPressedCallback)
                .def_prop_rw("range", &DoubleEditShuttle::getRange, nb::overload_cast<const RangeD&>(&DoubleEditShuttle::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<double, double>(&DoubleEditShuttle::setRange))
                .def_prop_rw("step", &DoubleEditShuttle::getStep, &DoubleEditShuttle::setStep)
                .def_prop_rw("largeStep", &DoubleEditShuttle::getLargeStep, &DoubleEditShuttle::setLargeStep)
                .def_prop_rw("defaultValue", &DoubleEditShuttle::getDefault, &DoubleEditShuttle::setDefault)
                .def("getModel", &DoubleEditShuttle::getModel)
                .def_prop_rw("precision", &DoubleEditShuttle::getPrecision, &DoubleEditShuttle::setPrecision)
                .def_prop_rw("font", &DoubleEditShuttle::getFont, &DoubleEditShuttle::setFont);
        }
    }
}
