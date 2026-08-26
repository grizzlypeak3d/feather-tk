// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IContainer.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        class PyIContainer : public IContainer
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

            Size2I getSizeHint() const override
            {
                PYBIND11_OVERRIDE(
                    Size2I,
                    IContainer,
                    getSizeHint);
            }

            void setGeometry(const Box2I& value) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IContainer,
                    setGeometry,
                    value);
            }

            void tickEvent(
                bool parentsVisible,
                bool parentsEnabled,
                const TickEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IContainer,
                    tickEvent,
                    parentsVisible,
                    parentsEnabled,
                    event);
            }

            void styleEvent(const StyleEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IContainer,
                    styleEvent,
                    event);
            }

            void sizeHintEvent(const SizeHintEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IContainer,
                    sizeHintEvent,
                    event);
            }

            void drawEvent(const Box2I& drawRect, const DrawEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IContainer,
                    drawEvent,
                    drawRect,
                    event);
            }

            void drawOverlayEvent(const Box2I& drawRect, const DrawEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IContainer,
                    drawOverlayEvent,
                    drawRect,
                    event);
            }

            void dragEnterEvent(DragDropEvent& event) override
            {
                // By hand rather than PYBIND11_OVERRIDE: the macro copies a
                // reference argument, so Python would accept a copy of the
                // event and the window would never hear.
                pybind11::gil_scoped_acquire gil;
                pybind11::function override =
                    pybind11::get_override(this, "dragEnterEvent");
                if (override)
                {
                    override(pybind11::cast(
                        &event, pybind11::return_value_policy::reference));
                }
                else
                {
                    IContainer::dragEnterEvent(event);
                }
            }

            void dragLeaveEvent(DragDropEvent& event) override
            {
                // By hand rather than PYBIND11_OVERRIDE: the macro copies a
                // reference argument, so Python would accept a copy of the
                // event and the window would never hear.
                pybind11::gil_scoped_acquire gil;
                pybind11::function override =
                    pybind11::get_override(this, "dragLeaveEvent");
                if (override)
                {
                    override(pybind11::cast(
                        &event, pybind11::return_value_policy::reference));
                }
                else
                {
                    IContainer::dragLeaveEvent(event);
                }
            }

            void dragMoveEvent(DragDropEvent& event) override
            {
                // By hand rather than PYBIND11_OVERRIDE: the macro copies a
                // reference argument, so Python would accept a copy of the
                // event and the window would never hear.
                pybind11::gil_scoped_acquire gil;
                pybind11::function override =
                    pybind11::get_override(this, "dragMoveEvent");
                if (override)
                {
                    override(pybind11::cast(
                        &event, pybind11::return_value_policy::reference));
                }
                else
                {
                    IContainer::dragMoveEvent(event);
                }
            }

            void dropEvent(DragDropEvent& event) override
            {
                // By hand rather than PYBIND11_OVERRIDE: the macro copies a
                // reference argument, so Python would accept a copy of the
                // event and the window would never hear.
                pybind11::gil_scoped_acquire gil;
                pybind11::function override =
                    pybind11::get_override(this, "dropEvent");
                if (override)
                {
                    override(pybind11::cast(
                        &event, pybind11::return_value_policy::reference));
                }
                else
                {
                    IContainer::dropEvent(event);
                }
            }
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
