# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

"""feather-tk is a lightweight toolkit for building professional tools for
film, VFX, and animation.

The API is the C++ one, compiled into the _feather_tk module and gathered
here, so that the package can also carry the libraries the module is built
on -- and the headers and CMake files to build against them.
"""

import os as _os
import sys as _sys

_here = _os.path.dirname(_os.path.abspath(__file__))

# Python 3.8 stopped finding an extension module's DLLs through PATH on
# Windows, so the directories they are in are named here, before the module
# is imported: bin in a wheel, and the prefix's bin when the package is
# installed to <prefix>/lib/feather_tk. The handles are kept, since closing
# one takes its directory away again.
_dll_directories = []
if _sys.platform == "win32":
    for _dir in (
            _os.path.join(_here, "bin"),
            _os.path.join(_here, _os.pardir, _os.pardir, "bin")):
        if _os.path.isdir(_dir):
            _dll_directories.append(_os.add_dll_directory(_dir))

from ._feather_tk import *
from ._feather_tk import __doc__

__version__ = VERSION_FULL


def get_cmake_dir():
    """The directory of feather-tk's CMake package in a wheel, for
    CMAKE_PREFIX_PATH or ftk_DIR when building against this installation."""
    return _os.path.join(_here, "lib", "cmake", "ftk")
