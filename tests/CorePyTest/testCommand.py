# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest


# ---------------------------------------------------------------------------
# Concrete command implementations used across tests
# ---------------------------------------------------------------------------

class AddCommand(ftk.ICommand):
    """Adds a value to a list; undo removes it."""
    def __init__(self, value, target):
        super().__init__()
        self._value = value
        self._target = target

    def exec(self):
        self._target.append(self._value)

    def undo(self):
        self._target.remove(self._value)


class SetCommand(ftk.ICommand):
    """Sets a mutable container's single value; stores previous for undo."""
    def __init__(self, new_value, container):
        super().__init__()
        self._new = new_value
        self._old = container[0]
        self._container = container

    def exec(self):
        self._container[0] = self._new

    def undo(self):
        self._container[0] = self._old


class CommandStack(ftk.CommandStack):
    """Thin wrapper that keeps Python command objects alive for the duration
    they are on the stack. This is a belt-and-suspenders complement to the
    py::keep_alive<1,2> annotation in Command.cpp — the binding-level policy
    is the real fix; this wrapper makes the intent explicit in the tests."""

    def __init__(self):
        super().__init__()
        self._refs = []

    def push(self, command):
        self._refs.append(command)
        super().push(command)

    def clear(self):
        self._refs.clear()
        super().clear()


# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

class CommandSubclassTest(unittest.TestCase):
    """Verify that Python subclasses of ICommand dispatch correctly."""

    def test_exec_and_undo(self):
        target = []
        cmd = AddCommand(42, target)
        cmd.exec()
        self.assertEqual(target, [42])
        cmd.undo()
        self.assertEqual(target, [])

    def test_set_command(self):
        container = [0]
        cmd = SetCommand(99, container)
        cmd.exec()
        self.assertEqual(container[0], 99)
        cmd.undo()
        self.assertEqual(container[0], 0)


class CommandStackBasicTest(unittest.TestCase):
    """Basic push / undo / redo behaviour."""

    def setUp(self):
        self.stack = CommandStack()
        self.value = [0]

    def _push(self, delta):
        self.stack.push(SetCommand(self.value[0] + delta, self.value))

    def test_push_executes_immediately(self):
        self.stack.push(SetCommand(5, self.value))
        self.assertEqual(self.value[0], 5)

    def test_undo(self):
        self.stack.push(SetCommand(1, self.value))
        self.stack.push(SetCommand(2, self.value))
        self.stack.undo()
        self.assertEqual(self.value[0], 1)

    def test_redo(self):
        self.stack.push(SetCommand(1, self.value))
        self.stack.push(SetCommand(2, self.value))
        self.stack.undo()
        self.stack.redo()
        self.assertEqual(self.value[0], 2)

    def test_full_undo_sequence(self):
        target = []
        for v in [1, 2, 3]:
            self.stack.push(AddCommand(v, target))
        self.assertEqual(target, [1, 2, 3])

        self.stack.undo()
        self.assertEqual(target, [1, 2])
        self.stack.undo()
        self.assertEqual(target, [1])
        self.stack.undo()
        self.assertEqual(target, [])

    def test_full_redo_sequence(self):
        target = []
        for v in [1, 2, 3]:
            self.stack.push(AddCommand(v, target))

        self.stack.undo()
        self.stack.undo()
        self.stack.undo()
        self.assertEqual(target, [])

        self.stack.redo()
        self.assertEqual(target, [1])
        self.stack.redo()
        self.assertEqual(target, [1, 2])
        self.stack.redo()
        self.assertEqual(target, [1, 2, 3])

    def test_push_after_undo_clears_redo(self):
        # Pushing a new command after undoing should discard the redo history.
        target = []
        self.stack.push(AddCommand(1, target))
        self.stack.push(AddCommand(2, target))
        self.stack.undo()            # back to [1]
        self.stack.push(AddCommand(9, target))  # branch: [1, 9]
        # redo is now unavailable — redoing should be a no-op
        self.stack.redo()
        self.assertEqual(target, [1, 9])

    def test_undo_empty_stack_is_noop(self):
        self.stack.undo()  # should not raise
        self.stack.undo()

    def test_redo_empty_stack_is_noop(self):
        self.stack.redo()  # should not raise

    def test_clear(self):
        target = []
        self.stack.push(AddCommand(1, target))
        self.stack.push(AddCommand(2, target))
        self.stack.clear()
        # After clear, undo and redo should both be no-ops
        self.stack.undo()
        self.assertEqual(target, [1, 2])
        self.stack.redo()
        self.assertEqual(target, [1, 2])


class CommandStackObservableTest(unittest.TestCase):
    """hasUndo / hasRedo observables reflect stack state correctly."""

    def setUp(self):
        self.stack = CommandStack()
        self.has_undo = [False]
        self.has_redo = [False]
        self.undo_observer = ftk.BoolObserver(
            self.stack.observeHasUndo(),
            lambda v: self.has_undo.__setitem__(0, v))
        self.redo_observer = ftk.BoolObserver(
            self.stack.observeHasRedo(),
            lambda v: self.has_redo.__setitem__(0, v))

    def test_initial_state(self):
        self.assertFalse(self.has_undo[0])
        self.assertFalse(self.has_redo[0])

    def test_has_undo_after_push(self):
        self.stack.push(AddCommand(1, []))
        self.assertTrue(self.has_undo[0])
        self.assertFalse(self.has_redo[0])

    def test_has_redo_after_undo(self):
        target = []
        self.stack.push(AddCommand(1, target))
        self.stack.undo()
        self.assertFalse(self.has_undo[0])
        self.assertTrue(self.has_redo[0])

    def test_no_redo_after_redo(self):
        target = []
        self.stack.push(AddCommand(1, target))
        self.stack.undo()
        self.stack.redo()
        self.assertTrue(self.has_undo[0])
        self.assertFalse(self.has_redo[0])

    def test_clear_resets_both(self):
        target = []
        self.stack.push(AddCommand(1, target))
        self.stack.push(AddCommand(2, target))
        self.stack.undo()
        self.stack.clear()
        self.assertFalse(self.has_undo[0])
        self.assertFalse(self.has_redo[0])

    def test_push_after_undo_clears_redo_observable(self):
        target = []
        self.stack.push(AddCommand(1, target))
        self.stack.undo()
        self.assertTrue(self.has_redo[0])
        self.stack.push(AddCommand(2, target))
        self.assertFalse(self.has_redo[0])


if __name__ == '__main__':
    unittest.main()
