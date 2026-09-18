# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

"""Build a C++ program against the installed feather_tk package, and run it.

The wheel carries feather-tk's CMake package so that a project with its own
bindings -- tlRender, say -- can build against the libraries in it. That
only works if the package finds its own files, including the plain library
names CMake's find modules look for, which nothing in the Python tests would
notice. Run by cibuildwheel after the tests, with cmake from pip.
"""

import os
import shutil
import subprocess
import sys
import tempfile

import feather_tk

HERE = os.path.dirname(os.path.abspath(__file__))


def run(*args, env=None):
    print(" ".join(args), flush=True)
    subprocess.run(args, check=True, env=env)


def main():
    cmake = shutil.which("cmake")
    if not cmake:
        raise RuntimeError("cmake is not on PATH")
    with tempfile.TemporaryDirectory() as build:
        run(cmake, "-S", HERE, "-B", build,
            "-DCMAKE_BUILD_TYPE=Release",
            "-Dftk_DIR=" + feather_tk.get_cmake_dir())
        run(cmake, "--build", build, "--config", "Release")

        env = dict(os.environ)
        exe = os.path.join(build, "wheel-consumer")
        if sys.platform == "win32":
            exe = os.path.join(build, "Release", "wheel-consumer.exe")
            # Windows finds a program's DLLs on PATH: the package's, and the
            # C++ runtime delvewheel put beside it.
            here = os.path.dirname(feather_tk.__file__)
            dirs = [os.path.join(here, "bin"), here + ".libs"]
            env["PATH"] = os.pathsep.join(dirs + [env.get("PATH", "")])
        run(exe, env=env)


if __name__ == "__main__":
    main()
