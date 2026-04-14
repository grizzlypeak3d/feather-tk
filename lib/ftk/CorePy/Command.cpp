// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Command.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        // Trampoline class so Python can subclass ICommand and override
        // exec() and undo().
        class PyICommand : public ICommand
        {
        public:
            void exec() override
            {
                PYBIND11_OVERRIDE_PURE(
                    void,
                    ICommand,
                    exec);
            }

            void undo() override
            {
                PYBIND11_OVERRIDE_PURE(
                    void,
                    ICommand,
                    undo);
            }
        };

        void command(py::module_& m)
        {
            py::class_<ICommand, PyICommand, std::shared_ptr<ICommand> >(m, "ICommand")
                .def(py::init<>())
                .def("exec", &ICommand::exec)
                .def("undo", &ICommand::undo);

            py::class_<CommandStack, std::shared_ptr<CommandStack> >(m, "CommandStack")
                .def(py::init(&CommandStack::create))
                .def(
                    "push",
                    &CommandStack::push,
                    py::arg("command"),
                    // Keep the Python command object alive for at least as
                    // long as the CommandStack — prevents GC from collecting
                    // the subclass instance before undo() is called.
                    py::keep_alive<1, 2>())
                .def("clear", &CommandStack::clear)
                .def("undo", &CommandStack::undo)
                .def("redo", &CommandStack::redo)
                .def("observeHasUndo", &CommandStack::observeHasUndo)
                .def("observeHasRedo", &CommandStack::observeHasRedo);
        }
    }
}
