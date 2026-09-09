# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import os
import sys
import unittest

@unittest.skipIf(os.environ.get("FTK_TESTS_NO_GL"), "OpenGL is not available")
class AppTest(unittest.TestCase):

    def setUp(self):
        self.context = ftk.Context()
        ftk.uiInit(self.context)

    def test_ownership(self):
        # Python is the only owner of an application, before and after
        # it runs: an application that is built and never run ("-h")
        # goes away when the script lets it go. The windows find the
        # application through their weak references all the same.
        probe = ftk.Context()
        app = ftk.App(self.context, ["app", "-exit"], "ftkPyTest", "Test application")
        self.assertEqual(sys.getrefcount(app), sys.getrefcount(probe))
        window = ftk.Window(self.context, app, "ftkPyTest")
        self.assertIs(window.app, app)
        self.assertIs(app.windows[0], window)
        app.run()
        self.assertIs(window.app, app)
        self.assertEqual(sys.getrefcount(app), sys.getrefcount(probe))

    def test_parentAfterChildren(self):
        # A Python widget that is given children before it is given a
        # parent: the children still find it, and the window through it.
        app = ftk.App(self.context, ["app", "-exit"], "ftkPyTest", "Test application")
        window = ftk.Window(self.context, app, "ftkPyTest")
        class Container(ftk.IContainer):
            def __init__(self, context, parent = None):
                ftk.IContainer.__init__(self, context, "Container", parent)
        container = Container(self.context)
        combo = ftk.ComboBox(self.context, ["A", "B"], container)
        container.parent = window
        self.assertIs(combo.parent, container)
        self.assertIs(combo.window, window)

    def test_neverRun(self):
        freed = []
        class App(ftk.App):
            def __del__(self):
                freed.append(True)
        app = App(self.context, ["app"], "ftkPyTest", "Test application")
        self.assertFalse(app.hasCmdLineHelp)
        del app
        self.assertTrue(freed)
