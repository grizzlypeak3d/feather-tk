# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class TimerTest(unittest.TestCase):

    def setUp(self):
        self.context = ftk.Context()

    def test_create(self):
        timer = ftk.Timer(self.context)
        self.assertIsNotNone(timer)
        self.assertFalse(timer.active)

    def test_start_stop(self):
        timer = ftk.Timer(self.context)
        self.assertFalse(timer.active)

        timer.start(0.001, lambda: None)
        self.assertTrue(timer.active)

        timer.stop()
        self.assertFalse(timer.active)

    def test_repeating(self):
        timer = ftk.Timer(self.context)
        self.assertFalse(timer.repeating)
        timer.repeating = True
        self.assertTrue(timer.repeating)
        timer.repeating = False
        self.assertFalse(timer.repeating)

    def test_timeout(self):
        timer = ftk.Timer(self.context)
        timer.start(0.1, lambda: None)
        self.assertGreaterEqual(timer.timeout.total_seconds(), 0)
        timer.stop()

if __name__ == '__main__':
    unittest.main()
