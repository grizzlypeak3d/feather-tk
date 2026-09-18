# SPDX-License-Identifier: BSD-3-Clause

"""Run an example with the built feather_tk findable.

    run-example.py <library directory> <script> [arguments...]

The same reason as tests/run-python-tests.py: Python 3.8 stopped resolving
an extension module's dependencies through PATH on Windows, so a shared
build's feather_tk cannot find the libraries under it unless
os.add_dll_directory() has been told where they are. Elsewhere the loader
follows the rpath recorded in the libraries and the directory is ignored.

The script runs as "__main__" with the arguments after it as its own, so
it sees exactly what it would see when run directly.
"""

import os
import runpy
import sys

directory = sys.argv[1]
if hasattr(os, "add_dll_directory") and os.path.isdir(directory):
    os.add_dll_directory(directory)

script = sys.argv[2]
sys.argv = sys.argv[2:]
runpy.run_path(script, run_name="__main__")
