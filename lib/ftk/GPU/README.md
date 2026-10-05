# The GPU renderer

A second renderer, beside the OpenGL one, on SDL's GPU API: Metal on macOS and
Vulkan on Linux and Windows. It exists for what OpenGL cannot give on Linux
and Windows, an HDR swapchain, and as the way off OpenGL where OpenGL is
deprecated.

OpenGL and OpenGL ES stay. OpenGL is what draws unless this renderer is asked
for, and the Raspberry Pi's OpenGL ES and the web have nothing else.

## Building

`ftk_GPU` builds it. It needs SDL3 (`ftk_SDL3`), built with its Metal and
Vulkan drivers, which the super build does. `etc/Config/default.cmake` turns
it on except with OpenGL ES or SDL2.

Vulkan takes SPIR-V, so builds for Vulkan also need glslang to compile the
shaders. The super build builds it everywhere but macOS, where Metal compiles
its own language; set `ftk_glslang` there to check the GLSL as well. A build
for Vulkan without glslang has the renderer and no shaders for it: it says so
when asked to start, and OpenGL draws.

A build tree from before the option existed has it in its cache as OFF, and a
configuration file does not overwrite what is there:

    cmake -S . -B build -Dftk_GPU=ON

At run time Vulkan wants the system's loader (`libvulkan.so.1`, or
`vulkan-1.dll`) and a driver.

## Choosing it

OpenGL draws unless this renderer is asked for. An application asks in this
order:

1. `-renderer GPU` or `-renderer OpenGL` on the command line, for that run.
   `-renderer OpenGL` is the way back from a renderer that cannot draw the
   application's settings.
2. The `FTK_RENDER` environment variable: `gpu`, or anything else for OpenGL.
3. `ftk::App::setRenderer()`, which is kept with the settings (`/Renderer`)
   and is for the next start.

The renderer is chosen as the application starts, after its settings are read
and before there is a window. `ftk::gpu::start()` makes the device, and
`ftk::gpu::isEnabled()` says whether this renderer draws. A program without
an `ftk::App` asks `ftk::gpu::isRequested()`, which reads `FTK_RENDER`, and
calls `start()` itself.

Where it is asked for and cannot draw, the log says why and OpenGL draws.
That is the case when:

- No device can be made: there is no driver for Vulkan, say.
- The only device draws on the CPU (Mesa's lavapipe). `FTK_GPU_SOFTWARE=1`
  takes one all the same, to test on a machine without a GPU.
- The build has no shader compiler for the device.

The device is asked only for what the renderer uses. SDL asks a Vulkan device
for depth clamping, clip distances, anisotropic filtering and indirect draws'
first instance unless told not to, and refuses a device without one; none is
used here, so none is asked for, and every pipeline clips by depth, which is
what going without depth clamping requires. SDL still requires independent
blending, sample rate shading and cube map arrays.

OpenGL is chosen and loaded only where it draws: `ftk::gl::System::init()`
starts SDL's video subsystem and `initGL()` does the rest.

## What is where

- `System`: the device, and the numbers that stand for textures where
  `IRender::drawTexture()` takes one.
- `Texture`, `OffscreenBuffer`: what they are in the OpenGL renderer.
  `OffscreenBuffer::read()` is what `glReadPixels` is there.
- `Render`: an `IRender`. `drawShader()` and `setShader()` are for a renderer
  built on this one, such as tlRender's.
- `Present`: draws a window's buffer into its swapchain, SDR or HDR.
- `Shader`: the two shader languages, and glslang.
- `ftk/UI/WindowGL.cpp`: `_updateGPU()`, and a window with no OpenGL context.

## How it differs from the OpenGL renderer

- Draws are recorded into a command buffer. Vertices are gathered and sent
  when the frame ends, in a command buffer submitted first; textures are sent
  in command buffers of their own. A texture reused within a frame is
  "cycled", so that an earlier draw keeps what it was given.
- There is no bound frame buffer. `pushTarget()` and `popTarget()` draw into
  another buffer for a while; each ends a render pass.
- A buffer's first row is the top one. `drawTexture()` takes its "mirror"
  flag as callers mean it for OpenGL, and turns it around.
- There are no three channel textures: interleaved RGB is given a fourth
  channel as it is sent. Planar RGB and YUV are not, being planes.
- There are no depth or stencil targets.
- Every shader is written twice, in Metal's language and in GLSL for Vulkan
  (`RenderShaders.cpp`). `FTK_GPU_VALIDATE=1` compiles the GLSL of each as it
  is made whatever the driver, which is how the GLSL is checked on macOS.

## Texture formats

Vulkan leaves some formats to the driver.

- Sixteen bit normalized textures, which video over eight bits uses: where a
  device has none they are kept as half float, which every driver has and
  filters. A half holds eleven bits or so near one. `hasUNorm16()`;
  `FTK_GPU_NO_UNORM16=1` says there are none, to try it.
- Filtering of thirty-two bit float textures, which a lookup table kept in
  one relies on: SDL has no way to ask, so it is tried, by drawing two texels
  into one pixel. `hasFloatFilter()`; `FTK_GPU_NO_FLOAT_FILTER=1` says it
  cannot. **Nothing falls back from it yet**: such a device reads the nearest
  entry.

The log's `Texture formats:` and `Float texture filtering:` lines say what a
device lacks.

## HDR

A window's buffer holds what it always has: display encoded for sRGB, with
one as the white of the user interface. Where the swapchain is HDR, what is
drawn above one is brighter than white by the same curve, and `Present`
writes it into the swapchain in the swapchain's terms.

- The swapchain follows the display: where the display is showing HDR it is
  extended linear if the window can have that, and HDR10 if it cannot.
  `FTK_GPU_SWAPCHAIN` asks for one by name.
- `IWindow::getHDR()` says whether the window is HDR, how far above white the
  display goes, and what white is in nits where the system says.
- macOS: extended linear. The system does not say what white is in nits.
- Windows: extended linear. The system says where SDR white is.
- Linux: only a Wayland session gives HDR, and what it gives is HDR10. The
  log's `Video driver:` has to say `wayland`, not `x11`
  (`SDL_VIDEO_DRIVER=wayland` asks for it). SDL reports eighty nits for
  white there whatever it is; a PQ surface has its white at 203 nits, which
  the compositor takes to the white of everything else on the display, so
  that is where the presenter puts it (`getSDRWhiteLevel()`). The headroom
  SDL reports is the range of PQ, not what the display can do.

Not done: HLG, and HDR metadata for the swapchain, which SDL has no way to
set. What is brighter than the display goes is left to the display.

## Environment variables

| Variable | |
|---|---|
| `FTK_RENDER=gpu` | Draw with this renderer; anything else, with OpenGL whatever is set. |
| `FTK_GPU_SWAPCHAIN=sdr`, `hdr` or `hdr10` | Ask for a swapchain by name: SDR, extended linear or HDR10. |
| `FTK_GPU_HDR_TEST=1` | Draw patches at one, two, four and eight times white along the top of each window. |
| `FTK_GPU_DEBUG=1` | Turn on the API's validation. Set `SDL_ASSERT=abort` with it for a run nobody is watching: an assertion is otherwise a dialog. |
| `FTK_GPU_VALIDATE=1` | Compile every shader's GLSL as it is made, whatever the driver. |
| `FTK_GPU_NO_UNORM16=1` | Say the device has no sixteen bit normalized textures. |
| `FTK_GPU_NO_FLOAT_FILTER=1` | Say the device does not filter thirty-two bit float textures. |
| `FTK_GPU_SOFTWARE=1` | Take a device that draws on the CPU. |
| `FTK_GPU_SERIALIZE=1` | Wait for the vertices and textures sent to the device before submitting what draws with them. For finding out whether a driver orders the two itself; it costs the frame the wait. |
| `SDL_GPU_DRIVER=vulkan` | SDL's own: which driver. |

The ones that turn something on are off when unset, empty or `0`.

With `-log`, look for `GPU driver:`, `GPU device:`, `GLSL compiler:`,
`Texture formats:`, `Float texture filtering:` and `Swapchain:`.

## Checking it

`ftk_TESTS` builds `ftk-gpu-test`:

    ftk-gpu-test [directory for the pictures]

It draws one scene with both renderers and compares them, checks that a
buffer without a size is refused, reads a buffer back as every type of image
a file is written from, checks the presenter's HDR arithmetic, and presents
into each kind of swapchain the desktop offers. It ends with `PASS` or
`FAIL`. On Vulkan it shows its window, since SDL's Vulkan driver has no
swapchain texture for a hidden one, and says how fast frames are presented.

Any application can be compared with itself:

    app -screenshot gl.png
    app -renderer GPU -screenshot gpu.png
    ftk-gpu-test -compare gl.png gpu.png [difference.png]

The Diagnostics has `ftk GPU Objects` and `ftk GPU Memory` beside the OpenGL
ones, which read zero while this renderer draws.

## Known differences from OpenGL

- The edges of glyphs: OpenGL's sixteen bit texture coordinates show. About
  0.2% of an application's channels are off by more than two, and none by
  more than about 32.
- An edge that falls exactly on the centers of a row of pixels: OpenGL draws
  that row at the bottom of a rectangle and not at the top, and Metal and
  Vulkan the other way around, since OpenGL's buffers are the other way up.
  A rectangle placed there is a row shorter or a row away.
- The same with the Nearest filter: where the center of a pixel falls exactly
  between two texels the two take different ones.

## Where it has run

| Driver | Machine | Checked by |
|---|---|---|
| Metal | Apple M1 Max, macOS | `ftk-gpu-test`, and applications by screenshot |
| Vulkan, Mesa RADV | Radeon RX 480, Linux on Wayland, HDR display | the same, and by eye in an HDR10 swapchain |
| Vulkan, Mesa ANV | Intel UHD 620, Linux on Wayland | by eye and by log |
| Vulkan, NVIDIA | RTX A4000, Windows, HDR display | by eye and by log, in an extended linear swapchain |
| Vulkan, Mesa V3DV | Raspberry Pi 5 (V3D 7.1), Linux on X11 | by eye and by log; see below |

Each has every texture format. The Raspberry Pi's does not filter float
textures, and what it draws is now and then wrong in a picture that is
playing: a frame with a piece of the picture where the picture should be.
That is not understood yet; `FTK_GPU_SERIALIZE=1` is for finding out whether
it is the order the driver does things in.

With `FTK_GPU_DEBUG=1` Vulkan's validation has
one thing to say, once, when a three dimensional texture is made:
`WARNING-VkImageSubresourceRange-layerCount-compatibility`, of a barrier
SDL's driver makes. It is SDL's.

## Not done

- A fallback for a device that does not filter float textures.
- Choosing the device where there are two: SDL takes the best kind it finds,
  and has nothing to choose with beyond preferring low power.
- Direct3D 12, which SDL also has, and which would want a third shader
  language.
- Depth and stencil targets.
