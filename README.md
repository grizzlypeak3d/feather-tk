[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Build Status](https://github.com/grizzlypeak3d/feather-tk/actions/workflows/ci-workflow.yml/badge.svg)](https://github.com/grizzlypeak3d/feather-tk/actions/workflows/ci-workflow.yml)
[![codecov](https://codecov.io/gh/codecov/example-cpp11-cmake/branch/master/graph/badge.svg)](https://codecov.io/gh/grizzlypeak3d/feather-tk)

# ![feather-tk Icon](etc/Icons/feather_tk_32.svg) feather-tk

Feather-tk is a lightweight toolkit for building cross-platform C++ applications.

Projects using feather-tk:
* https://github.com/grizzlypeak3d/DJV
* https://github.com/grizzlypeak3d/tlRender
* https://github.com/OpenTimelineIO/toucan

Example image viewer showing menus, toolbars, and persistent settings:

![DemoDarkStyle](etc/Images/imageview.png)

Example object viewer showing offscreen rendering and a heads up display:

![DemoDarkStyle](etc/Images/objview.png)

Example text editor showing multiple documents with loading and saving:

![DemoDarkStyle](etc/Images/textedit.png)

Simple C++ example that shows a window with a text label:
```
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
    label->setFontRole(FontRole::Title);
    label->setAlign(HAlign::Center, VAlign::Center);
    window->setWidget(label);

    // Run the application.
    app->run();
    return 0;
}
```

Simple Python exmple that shows a window with a text label (Python bindings
are a work in progress):
```
import ftkPy as ftk
import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "simple", "Simple example")
if app.getExit() != 0:
    sys.exit(app.getExit())

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a label.
label = ftk.Label(context, "Hello world")
label.fontRole = ftk.FontRole.Title
label.setAlign(ftk.HAlign.Center, ftk.VAlign.Center)
window.setWidget(label)

# Run the application.
app.run()

# \bug Need to manually reset the window.
window = None

```


## Building Dependencies

A CMake super build script is provided to build all of the dependencies from
source.

Required dependencies:
* [Freetype](https://freetype.org/)
* [GLFW](https://www.glfw.org/)
* [LunaSVG](https://github.com/sammycage/lunasvg)
* [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended)
* [nlohmann JSON](https://github.com/nlohmann/json)
* [PNG](http://www.libpng.org/pub/png/libpng.html)
* [ZLIB](https://zlib.net/)

Optional dependencies:
* [pybind11](https://github.com/pybind/pybind11)


## Building on Linux

Dependencies:
* CMake 3.31

Install system packages (Debian based systems):
```
sudo apt-get install xorg-dev libglu1-mesa-dev mesa-common-dev mesa-utils
```

Install system packages (Rocky 9):
```
sudo dnf install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel mesa-libGL-devel
```

Clone the repository:
```
git clone https://github.com/grizzlypeak3d/feather-tk.git
```

Run the super build script:
```
sh feather-tk/sbuild-linux.sh
```

Try running the `objview` example:
```
build-Release/examples/objview/objview feather-tk\etc\Objects\Bolt.obj
```


## Building on macOS

Dependencies:
* Xcode
* CMake 3.31

Clone the repository:
```
git clone https://github.com/grizzlypeak3d/feather-tk.git
```

Run the super build script:
```
sh feather-tk/sbuild-macos.sh
```

Try running the `objview` example:
```
build-Release/examples/objview/objview feather-tk\etc\Objects\Bolt.obj
```

These aliases are convenient for switching between architectures:
```
alias arm="env /usr/bin/arch -arm64 /bin/zsh --login"
alias intel="env /usr/bin/arch -x86_64 /bin/zsh --login"
```


## Building on Windows

Dependencies:
* Visual Studio 2022
* CMake 3.31

Open the Visual Studio command console "x64 Native Tools Command Prompt for VS 2022".
This can be found in the Start menu, in the "Visual Studio 2022" folder.

Clone the repository:
```
git clone https://github.com/grizzlypeak3d/feather-tk.git
```

Run the super build script:
```
feather-tk\sbuild-win.bat
```

Try running the `objview` example:
```
build-Release\examples\objview\Release\objview feather-tk\etc\Objects\Bolt.obj
```
