# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class CmdLineTest(unittest.TestCase):

    def test_flag(self):
        flag = ftk.CmdLineFlag(["-v", "--verbose"], "Enable verbose output")
        self.assertFalse(flag.found)
        self.assertIn("verbose", flag.help.lower())

    def test_flag_with_group(self):
        flag = ftk.CmdLineFlag(["-h", "--help"], "Show help", "General")
        self.assertEqual(flag.group, "General")

    def test_option_int(self):
        opt = ftk.CmdLineOptionI(["-n", "--count"], "Item count", "General", 0)
        self.assertFalse(opt.found)

    def test_option_float(self):
        opt = ftk.CmdLineOptionF(["-s", "--scale"], "Scale factor", "General", 1.0)
        self.assertFalse(opt.found)

    def test_option_string(self):
        opt = ftk.CmdLineOptionString(["-o", "--output"], "Output file", "General", "")
        self.assertFalse(opt.found)

    def test_arg_string(self):
        arg = ftk.CmdLineArgString("input", "Input file", False)
        self.assertFalse(arg.hasValue)

    def test_list_arg_string(self):
        arg = ftk.CmdLineListArgString("files", "Input files", False)
        self.assertFalse(arg.list)

if __name__ == '__main__':
    unittest.main()
