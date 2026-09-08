// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatEditShuttle.h>

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
        void floatEditShuttle(nb::module_& m)
        {
            nb::class_<FloatEditShuttle, IContainer>(m, "FloatEditShuttle")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FloatEditShuttle::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&FloatEditShuttle::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &FloatEditShuttle::getValue, &FloatEditShuttle::setValue)
                .def("setCallback", &FloatEditShuttle::setCallback)
                .def("setPressedCallback", &FloatEditShuttle::setPressedCallback)
                .def_prop_rw("range", &FloatEditShuttle::getRange, nb::overload_cast<const RangeF&>(&FloatEditShuttle::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<float, float>(&FloatEditShuttle::setRange))
                .def_prop_rw("step", &FloatEditShuttle::getStep, &FloatEditShuttle::setStep)
                .def_prop_rw("largeStep", &FloatEditShuttle::getLargeStep, &FloatEditShuttle::setLargeStep)
                .def_prop_rw("defaultValue", &FloatEditShuttle::getDefault, &FloatEditShuttle::setDefault)
                .def("getModel", &FloatEditShuttle::getModel)
                .def_prop_rw("precision", &FloatEditShuttle::getPrecision, &FloatEditShuttle::setPrecision)
                .def_prop_rw("font", &FloatEditShuttle::getFont, &FloatEditShuttle::setFont);
        }
    }
}
