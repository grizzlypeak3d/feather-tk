// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/IContainer.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        class PyIContainer : public PyWidget<IContainer>
        {
        public:
            static std::shared_ptr<PyIContainer> create(
                const std::shared_ptr<Context>& context,
                const std::string& objectName,
                const std::shared_ptr<IWidget>& parent = nullptr)
            {
                auto out = std::shared_ptr<PyIContainer>(new PyIContainer);
                out->_init(context, objectName, parent);
                return out;
            }

            using IContainer::_setWidget;
        };

        void iContainer(py::module_& m)
        {
            py::class_<IContainer, IWidget, std::shared_ptr<IContainer>, PyIContainer>(m, "IContainer")
                .def(
                    py::init(&PyIContainer::create),
                    py::arg("context"),
                    py::arg("objectName"),
                    py::arg("parent") = nullptr)
                .def_property_readonly("widget", &IContainer::getWidget)
                .def(
                    "_setWidget",
                    [](IContainer& self, const std::shared_ptr<IWidget>& value)
                    {
                        static_cast<PyIContainer&>(self)._setWidget(value);
                    });
        }
    }
}
