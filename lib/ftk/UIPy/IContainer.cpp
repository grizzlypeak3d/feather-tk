// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/IContainer.h>

#include <nanobind/nanobind.h>
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
        class PyIContainer : public PyWidget<IContainer>
        {
        public:
            void pyInit(
                const std::shared_ptr<Context>& context,
                const std::string& objectName,
                const std::shared_ptr<IWidget>& parent)
            {
                _init(context, objectName, parent);
            }

            using IContainer::_setWidget;
        };

        void iContainer(nb::module_& m)
        {
            nb::class_<IContainer, IWidget, PyIContainer>(m, "IContainer")
                .def(
                    "__init__",
                    [](IContainer* self,
                       const std::shared_ptr<Context>& context,
                       const std::string& objectName,
                       const std::shared_ptr<IWidget>& parent)
                    {
                        pyConstruct<PyIContainer>(self,
                            [&](PyIContainer& w)
                            {
                                w.pyInit(context, objectName, parent);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("objectName"),
                    nb::arg("parent") = nullptr)
                .def_prop_ro("widget", &IContainer::getWidget)
                .def(
                    "_setWidget",
                    // Self as a shared_ptr: _setWidget parents through
                    // shared_from_this().
                    [](const std::shared_ptr<IContainer>& self,
                       const std::shared_ptr<IWidget>& value)
                    {
                        static_cast<PyIContainer&>(*self)._setWidget(value);
                    });
        }
    }
}
