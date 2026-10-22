# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import feather_tk as ftk

import unittest

class DiagSystemTest(unittest.TestCase):

    def setUp(self):
        self.context = ftk.Context()

    def test_group_order(self):
        # A system of its own, so the samplers are only its own and these.
        system = ftk.DiagSystem(self.context)
        self.assertEqual(["Memory", "Objects"], system.groups)
        system.addSampler("Test/A: {0}", lambda: 1)
        system.addSampler("Frame/Time: {0}", lambda: 2)
        self.assertEqual(["Frame", "Memory", "Objects", "Test"], system.groups)
        system.groupOrder = ["Test"] + system.groupOrder
        self.assertEqual("Test", system.groupOrder[0])
        self.assertEqual(["Test", "Frame", "Memory", "Objects"], system.groups)

if __name__ == '__main__':
    unittest.main()
