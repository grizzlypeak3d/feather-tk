# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class StringTest(unittest.TestCase):

    def test_getLoremIpsum(self):
        lines = ftk.getLoremIpsum()
        self.assertIsInstance(lines, list)
        self.assertGreater(len(lines), 0)
        for line in lines:
            self.assertIsInstance(line, str)
            self.assertGreater(len(line), 0)

    def test_getLoremIpsum_lineCount(self):
        text = ftk.getLoremIpsum(3)
        self.assertIsInstance(text, str)
        self.assertGreater(len(text), 0)
        # Three lines means at least two newlines separating them
        self.assertGreaterEqual(text.count('\n'), 2)

    def test_getLoremIpsum_single_line(self):
        text = ftk.getLoremIpsum(1)
        self.assertIsInstance(text, str)
        self.assertGreater(len(text), 0)

if __name__ == '__main__':
    unittest.main()
