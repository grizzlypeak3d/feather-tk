# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import gc
import unittest

class ObservableLifetimeTest(unittest.TestCase):
    """Tests for observer lifetime management and expired-observer pruning."""

    def test_observer_fires_on_create(self):
        # ObserverAction::Trigger (default): callback fires immediately with
        # the current value on subscription.
        received = []
        obs = ftk.ObservableInt(42)
        observer = ftk.IntObserver(obs, lambda v: received.append(v))
        self.assertEqual(received, [42])

    def test_observer_fires_on_change(self):
        received = []
        obs = ftk.ObservableInt(0)
        observer = ftk.IntObserver(obs, lambda v: received.append(v))
        received.clear()
        obs.setAlways(1)
        obs.setAlways(2)
        self.assertEqual(received, [1, 2])

    def test_setIfChanged_no_fire_when_equal(self):
        count = [0]
        obs = ftk.ObservableInt(5)
        observer = ftk.IntObserver(obs, lambda v: count.__setitem__(0, count[0] + 1))
        count[0] = 0
        self.assertFalse(obs.setIfChanged(5))
        self.assertEqual(count[0], 0)
        self.assertTrue(obs.setIfChanged(6))
        self.assertEqual(count[0], 1)

    def test_observer_stops_firing_after_deletion(self):
        # Once the observer is deleted, notifications should not reach the
        # old callback. This also exercises the expired-observer pruning path.
        received = []
        obs = ftk.ObservableInt(0)
        observer = ftk.IntObserver(obs, lambda v: received.append(v))
        received.clear()

        del observer
        gc.collect()

        obs.setAlways(99)
        self.assertEqual(received, [])

    def test_multiple_observers(self):
        results = [[], []]
        obs = ftk.ObservableInt(0)
        o1 = ftk.IntObserver(obs, lambda v: results[0].append(v))
        o2 = ftk.IntObserver(obs, lambda v: results[1].append(v))
        results[0].clear()
        results[1].clear()

        obs.setAlways(7)
        self.assertEqual(results[0], [7])
        self.assertEqual(results[1], [7])

    def test_partial_observer_deletion(self):
        # Deleting one observer should not affect another still alive.
        r1, r2 = [], []
        obs = ftk.ObservableInt(0)
        o1 = ftk.IntObserver(obs, lambda v: r1.append(v))
        o2 = ftk.IntObserver(obs, lambda v: r2.append(v))
        r1.clear()
        r2.clear()

        del o1
        gc.collect()

        obs.setAlways(3)
        self.assertEqual(r1, [])
        self.assertEqual(r2, [3])


class ObservableListMutationTest(unittest.TestCase):
    """Tests for ObservableList mutation methods and notifications."""

    def setUp(self):
        self.received = None
        self.obs = ftk.ObservableIntList([])
        self.observer = ftk.IntListObserver(
            self.obs, lambda v: setattr(self, 'received', v))
        self.received = None  # clear the trigger-on-subscribe value

    def test_pushBack_single(self):
        self.obs.pushBack(1)
        self.assertEqual(self.received, [1])
        self.obs.pushBack(2)
        self.assertEqual(self.received, [1, 2])

    def test_pushBack_list(self):
        self.obs.pushBack([10, 20, 30])
        self.assertEqual(self.received, [10, 20, 30])

    def test_setItem(self):
        self.obs.setAlways([1, 2, 3])
        self.obs.setItem(1, 99)
        self.assertEqual(self.received, [1, 99, 3])

    def test_setItemOnlyIfChanged(self):
        self.obs.setAlways([1, 2, 3])
        self.received = None
        self.assertFalse(self.obs.setItemOnlyIfChanged(0, 1))
        self.assertIsNone(self.received)
        self.assertTrue(self.obs.setItemOnlyIfChanged(0, 5))
        self.assertEqual(self.received[0], 5)

    def test_insertItem(self):
        self.obs.setAlways([1, 3])
        self.obs.insertItem(1, 2)
        self.assertEqual(self.received, [1, 2, 3])

    def test_insertItems(self):
        self.obs.setAlways([1, 4])
        self.obs.insertItems(1, [2, 3])
        self.assertEqual(self.received, [1, 2, 3, 4])

    def test_removeItem(self):
        self.obs.setAlways([1, 2, 3])
        self.obs.removeItem(1)
        self.assertEqual(self.received, [1, 3])

    def test_removeItems(self):
        self.obs.setAlways([1, 2, 3, 4])
        self.obs.removeItems(1, 3)
        self.assertEqual(self.received, [1, 4])

    def test_replaceItems(self):
        self.obs.setAlways([1, 2, 3, 4])
        self.obs.replaceItems(1, 3, [9, 8])
        self.assertEqual(self.received, [1, 9, 8, 4])

    def test_clear(self):
        self.obs.setAlways([1, 2, 3])
        self.obs.clear()
        self.assertEqual(self.received, [])

    def test_clear_no_notify_when_already_empty(self):
        self.obs.setAlways([])
        self.received = None
        self.obs.clear()
        self.assertIsNone(self.received)

    def test_contains_and_indexOf(self):
        self.obs.setAlways([10, 20, 30])
        self.assertTrue(self.obs.contains(20))
        self.assertFalse(self.obs.contains(99))
        self.assertEqual(self.obs.indexOf(10), 0)
        self.assertEqual(self.obs.indexOf(30), 2)


class ObservableMapMutationTest(unittest.TestCase):
    """Tests for ObservableStringIntMap."""

    def setUp(self):
        self.received = None
        self.obs = ftk.ObservableStringIntMap({})
        self.observer = ftk.StringIntMapObserver(
            self.obs, lambda v: setattr(self, 'received', v))
        self.received = None

    def test_set_and_notify(self):
        self.obs.setAlways({"x": 1})
        self.assertEqual(self.received, {"x": 1})

    def test_setIfChanged(self):
        self.obs.setAlways({"x": 1})
        self.received = None
        self.assertFalse(self.obs.setIfChanged({"x": 1}))
        self.assertIsNone(self.received)
        self.assertTrue(self.obs.setIfChanged({"x": 2}))
        self.assertEqual(self.received, {"x": 2})


if __name__ == '__main__':
    unittest.main()
