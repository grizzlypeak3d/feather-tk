[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Build Status](https://github.com/grizzlypeak3d/feather-tk/actions/workflows/ci-workflow.yml/badge.svg)](https://github.com/grizzlypeak3d/feather-tk/actions/workflows/ci-workflow.yml)

# ![feather-tk Icon](etc/Icons/feather_tk_32.png)&nbsp;feather-tk

A lightweight C++ toolkit for building professional tools for film, VFX, and animation.

feather-tk is purpose-built for media production software, with features like
high-bit-depth color and multi-monitor HiDPI support. It powers [DJV](https://github.com/grizzlypeak3d/DJV),
a production-proven image sequence viewer used in professional VFX and animation workflows.

---

## Why feather-tk?

**Lightweight and self-contained.** feather-tk has a small, well-defined set of
dependencies, and includes a CMake superbuild for building the dependencies.

**Modern C++ throughout.** Clean ownership model using `std::shared_ptr`, a reactive
observable system for UI state, and a consistent event-driven architecture that makes
writing custom widgets straightforward.

**BSD-3-Clause licensed.** Use it freely in commercial production tools.

---

## Used in production

| Project | Description |
|---------|-------------|
| [DJV](https://github.com/grizzlypeak3d/DJV) | Open source image sequence player for high-resolution film, VFX and animation workflows |
| [tlRender](https://github.com/grizzlypeak3d/tlRender) | Library for building playback and review applications for VFX, film, and animation |
| [toucan](https://github.com/OpenTimelineIO/toucan) | Software renderer for OpenTimelineIO timelines |

---

## Features

- **Widget toolkit** — full set of UI widgets including layouts, buttons, sliders, numeric
  editors, menus, toolbars, file browsers, dialogs, tab bars, scroll areas, and a MDI canvas.
- **Observable state** — reactive value, list, and map observables with automatic lifetime
  management; widgets subscribe and unsubscribe cleanly without manual bookkeeping.
- **Action system** — commands with text, icon, keyboard shortcut, checked state, and
  enabled state; a single `Action` drives menus, toolbars, and keyboard handling together.
- **Style and theming** — dark, light, and custom color styles; all sizes and colors are
  role-based so themes apply globally without touching widget code.
- **Settings persistence** — JSON-backed settings with typed get/set and automatic save
  on exit.
- **OpenGL rendering** — OpenGL 4.1 and OpenGL ES 2 backends.
- **HiDPI** — display scale awareness throughout; all size roles scale correctly on
  high-density displays and multi-monitor setups.
- **Python bindings** — pybind11-based Python API (work in progress).
- **Cross-platform** — works on Linux, macOS, and Windows.

---

## Quick start

Simple C++ example that shows a window with a text label:
```cpp
#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/MainWindow.h>

using namespace ftk;

int main(int argc, char** argv)
{
    // Create the context and application.
    auto context = Context::create();
    auto app = App::create(context, argc, argv, "simple", "Simple example");
    if (app->getExit() != 0)
        return app->getExit();

    // Create a window.
    auto window = MainWindow::create(context, app, Size2I(1280, 960));

    // Create a label.
    auto label = Label::create(context, "Hello world");
    label->setHAlign(HAlign::Center);
    window->setWidget(label);

    // Run the application.
    app->run();
    return 0;
}
```

Simple Python example that shows a window with a text label (Python bindings
are a work in progress):
```python
import ftkPy as ftk
import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "simple", "Simple example")
if app.exitValue != 0:
    sys.exit(app.exitValue)

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a label.
label = ftk.Label(context, "Hello world")
label.hAlign = ftk.HAlign.Center
window.widget = label

# Run the application.
app.run()

# Clean up.
window = None
app = None

```

---

## Examples

Image viewer with menus, toolbars, and persistent settings:

![Image viewer example](etc/Images/imageview.png)

3D object viewer with offscreen rendering and heads-up display:

![Object viewer example](etc/Images/objview.png)

Text editor with multiple documents:

![Text editor example](etc/Images/textedit.png)

More examples are in the [examples/](examples/) directory, covering widgets,
layouts, graphics, and Python bindings.

---

## Architecture overview

feather-tk is organised into three layers:

**Core** — context and system management, observable values, math and geometry types,
image I/O, font rendering, string utilities, file I/O, LRU cache, command/undo stack.

**GL** — OpenGL abstraction layer: shaders, textures, meshes, offscreen buffers, and
a render interface used by the UI layer.

**UI** — the widget toolkit: the `IWidget` base class and event system, all built-in
widgets, the style system, action system, settings, and the application event loop.

The observable pattern runs throughout. UI state — numeric model values, action checked
state, style changes, window focus — is all expressed as `Observable<T>` values that
widgets subscribe to. Observers unregister automatically on destruction, so there are
no manual disconnect calls and no dangling callbacks.

---

## Building

### Dependencies

Required:
- [Freetype](https://freetype.org/)
- [GLFW](https://www.glfw.org/)
- [LunaSVG](https://github.com/sammycage/lunasvg)
- [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended)
- [nlohmann JSON](https://github.com/nlohmann/json)
- [PNG](http://www.libpng.org/pub/png/libpng.html)
- [ZLIB](https://zlib.net/)

Optional:
- [pybind11](https://github.com/pybind/pybind11) — for Python bindings

A CMake superbuild script builds all dependencies from source automatically.

### Linux

Install system packages (Debian/Ubuntu):
```sh
sudo apt-get install build-essential git cmake xorg-dev libglu1-mesa-dev mesa-common-dev mesa-utils
```

Install system packages (Rocky 8 and 9):
```sh
sudo dnf install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel mesa-libGL-devel
```

Rocky 8 additionally requires a newer compiler:
```sh
sudo dnf install gcc-toolset-13
scl enable gcc-toolset-13 bash
```

Clone and build:
```sh
git clone https://github.com/grizzlypeak3d/feather-tk.git
sh feather-tk/sbuild-linux.sh
```

### macOS

Dependencies: Xcode, CMake 3.31

```sh
git clone https://github.com/grizzlypeak3d/feather-tk.git
sh feather-tk/sbuild-macos.sh
```

Notes for switching between architectures:
```sh
alias arm="env /usr/bin/arch -arm64 /bin/zsh --login"
alias intel="env /usr/bin/arch -x86_64 /bin/zsh --login"
```

### Windows

Dependencies: Visual Studio 2022, CMake 3.31

Open "x64 Native Tools Command Prompt for VS 2022" from the Start menu, then:
```bat
git clone https://github.com/grizzlypeak3d/feather-tk.git
feather-tk\sbuild-win.bat
```

### Verify the build

Run the object viewer example:
```sh
# Linux / macOS
build-Release/examples/objview/objview feather-tk/etc/Objects/Bolt.obj

# Windows
build-Release\examples\objview\Release\objview feather-tk\etc\Objects\Bolt.obj
```
