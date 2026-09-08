// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Command.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/trampoline.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        // Trampoline class so Python can subclass ICommand and override
        // exec() and undo().
        class PyICommand : public ICommand
        {
        public:
            NB_TRAMPOLINE(ICommand);

            void exec() override
            {
                NB_OVERRIDE_PURE(exec);
            }

            void undo() override
            {
                NB_OVERRIDE_PURE(undo);
            }
        };

        void command(nb::module_& m)
        {
            nb::class_<ICommand, PyICommand>(m, "ICommand")
                .def(nb::init<>())
                .def("exec", &ICommand::exec)
                .def("undo", &ICommand::undo);

            nb::class_<CommandStack>(m, "CommandStack")
                .def(nb::new_(&CommandStack::create))
                .def(
                    "push",
                    &CommandStack::push,
                    nb::arg("command"),
                    // Keep the Python command object alive for at least as
                    // long as the CommandStack — prevents GC from collecting
                    // the subclass instance before undo() is called.
                    nb::keep_alive<1, 2>())
                .def("clear", &CommandStack::clear)
                .def("undo", &CommandStack::undo)
                .def("redo", &CommandStack::redo)
                .def("observeHasUndo", &CommandStack::observeHasUndo)
                .def("observeHasRedo", &CommandStack::observeHasRedo);
        }
    }
}
