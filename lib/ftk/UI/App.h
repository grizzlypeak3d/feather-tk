// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/Style.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/IApp.h>
#include <ftk/Core/ObservableList.h>
#include <ftk/Core/ObservableMap.h>
#include <ftk/Core/Observable.h>

#include <chrono>
#include <filesystem>
#include <functional>
#include <list>

union SDL_Event;

namespace ftk
{
    class Capture;
    class FontSystem;
    class Settings;
    class IWidget;
    class IWindow;
    class Window;
    class IconSystem;
    struct TickEvent;

    //! Monitor information.
    struct FTK_UI_API_TYPE MonitorInfo
    {
        //! The monitor's name.
        std::string name;
        //! The size of the monitor's current display mode.
        Size2I      size;
        //! The refresh rate of the current display mode, in hertz; zero where
        //! it is not known.
        float       refreshRate = 0.F;
        //! The horizontal and vertical dots per inch. Not set in SDL3 builds,
        //! where it is zero.
        V2F         dpi;
        //! The monitor's area of the desktop, in screen coordinates.
        Box2I       bounds;

        bool operator == (const MonitorInfo&) const = default;
    };

    //! Color style.
    enum class FTK_UI_API_TYPE ColorStyle
    {
        Dark,
        Light,

        Count,
        First = Dark
    };
    FTK_ENUM(FTK_UI_API, ColorStyle);

    //! What an application's windows are drawn with.
    enum class FTK_UI_API_TYPE Renderer
    {
        OpenGL,
        //! The GPU renderer, Metal or Vulkan, where it is built and a
        //! device can be made for it, and OpenGL otherwise.
        GPU,

        Count,
        First = OpenGL
    };
    FTK_ENUM(FTK_UI_API, Renderer);

    //! Get the renderer an application draws with when nothing else is
    //! asked for: the GPU renderer on macOS, where it is Metal and OpenGL
    //! is deprecated, and OpenGL elsewhere. On a Mac with little else
    //! running, OpenGL in full screen had a swap held about once a second
    //! (30 to 140 hitches a minute playing at 24 and 30 fps), where Metal
    //! had none.
    FTK_UI_API Renderer getDefaultRenderer();

    //! Get whether playback is paced to the display when nothing else is
    //! asked for (see App::setDisplayPaced()): on macOS and Windows, where
    //! a capture of the display measured it, hitches a minute at 60, 30
    //! and 24 fps going from as many as 550 to a few or none; not on
    //! Linux, where Intel's OpenGL on Wayland played far worse paced
    //! (2300 a minute at 60 fps against 120), nor on iOS, whose loop is the
    //! display link already, nor the web, whose loop is the browser's.
    FTK_UI_API bool getDefaultDisplayPaced();

    //! Get display scales.
    FTK_UI_API std::vector<float> getDisplayScales();

    //! Where an application keeps its per-user files: the settings and the
    //! log. Left empty, no files are kept and the application is responsible
    //! for its own.
    //!
    //! A suite of applications gives them all the same directory and tells
    //! them apart by the base name, so that one directory holds the settings
    //! and logs of all of them rather than one directory apiece.
    struct FTK_UI_API_TYPE AppFiles
    {
        //! Directory under the user's documents. Empty keeps no files.
        std::string dirName;

        //! Base name for the settings and log files. Empty uses the
        //! application name.
        std::string baseName;

        //! Appended to the base name when greater than zero, so that two
        //! major versions of an application do not read each other's
        //! settings.
        int version = 0;
    };

    //! Base class for user interface applications.
    class FTK_UI_API_TYPE App : public IApp
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {},
            const AppFiles& = AppFiles());

        FTK_UI_API App();

    public:
        FTK_UI_API virtual ~App();

        //! Create a new application.
        FTK_UI_API static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            int argc, char** argv,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Create a new application.
        FTK_UI_API static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            int argc, wchar_t* argv[],
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Create a new application.
        FTK_UI_API static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Get the font system.
        FTK_UI_API const std::shared_ptr<FontSystem>& getFontSystem() const;

        //! Get the icon system.
        FTK_UI_API const std::shared_ptr<IconSystem>& getIconSystem() const;

        //! Get the style.
        FTK_UI_API const std::shared_ptr<Style>& getStyle() const;

        //! \name Files
        ///@{

        //! Get the settings. Null when no AppFiles was given.
        FTK_UI_API const std::shared_ptr<Settings>& getSettings() const;

        //! Get the settings file path. Empty when no AppFiles was given.
        FTK_UI_API const std::filesystem::path& getSettingsPath() const;

        //! Get the log file path. Empty when no AppFiles was given.
        FTK_UI_API const std::filesystem::path& getLogFilePath() const;

        //! Write a screenshot of the first window. Returns false when there
        //! is no window, when the offscreen buffer is not ready, or when the
        //! file cannot be written; the reason is logged.
        //!
        //! Only useful once the event loop has run: that is what realizes and
        //! sizes the window and leaves a buffer behind to read.
        FTK_UI_API bool writeScreenshot(const std::filesystem::path&);

        //! Write the first window's widget tree as JSON; see widgetDump().
        //! Returns false when there is no window or the file cannot be
        //! written; the reason is logged. Only useful once the event loop
        //! has run and laid the window out.
        FTK_UI_API bool writeWidgetDump(const std::filesystem::path&);

        //! Whether this run was started with the -captureShot option;
        //! see Capture.
        FTK_UI_API bool isCaptureRun() const;

        ///@}

        //! \name Color Style
        ///@{

        //! Get the color style. Dark unless the -colorStyle command line option
        //! or setColorStyle() says otherwise.
        FTK_UI_API ColorStyle getColorStyle() const;
        //! Observe the color style.
        FTK_UI_API std::shared_ptr<IObservable<ColorStyle> > observeColorStyle() const;
        FTK_UI_API void setColorStyle(ColorStyle);

        FTK_UI_API const std::shared_ptr<CmdLineOption<ColorStyle> >& getColorStyleCmdLineOption() const;

        ///@}

        //! \name Renderer
        ///@{

        //! Get the renderer that is set, which is getDefaultRenderer()
        //! unless the other is asked for. What draws is said by
        //! gpu::isEnabled(): this is
        //! what is kept with the settings, and where the GPU renderer
        //! cannot draw OpenGL does whatever was asked.
        FTK_UI_API Renderer getRenderer() const;
        //! Observe the renderer that is set.
        FTK_UI_API std::shared_ptr<IObservable<Renderer> > observeRenderer() const;

        //! Set the renderer. It is chosen as the application starts,
        //! before there is a window, so this is for the next time it
        //! does; it is kept with the settings. The command line is asked
        //! first and then the FTK_RENDER environment variable, so there
        //! is a way back from a renderer that cannot draw the settings.
        FTK_UI_API void setRenderer(Renderer);

        ///@}

        //! \name Pacing
        ///@{

        //! Get whether playback is paced to the display: while a system is
        //! busy (a player playing), the window showing the picture presents
        //! every refresh, so the loop goes round once a refresh in step with
        //! its display, and the frame is chosen for the refresh it will be
        //! shown at. getDefaultDisplayPaced() unless set; kept with the
        //! settings. The FTK_DISPLAY_PACED environment variable, 0 or 1,
        //! overrides it, for testing.
        FTK_UI_API bool isDisplayPaced() const;
        //! Observe whether playback is paced to the display.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeDisplayPaced() const;

        //! Set whether playback is paced to the display. It takes effect at
        //! once.
        FTK_UI_API void setDisplayPaced(bool);

        ///@}

        //! \name Display Scale
        ///@{

        //! Get the display scale found at startup: the first display's, or the
        //! -displayScale command line option's, and never less than one.
        FTK_UI_API float getDefaultDisplayScale() const;

        //! Get the display scale that setDisplayScale(0) gives: the window's
        //! once there is one, else the display's.
        FTK_UI_API float getAutoDisplayScale() const;
        //! Get the display scale. A window on a display of another scale can
        //! draw at that one instead; see getWindowDisplayScale().
        FTK_UI_API float getDisplayScale() const;
        //! Observe the display scale.
        FTK_UI_API std::shared_ptr<IObservable<float> > observeDisplayScale() const;
        //! Set the display scale. Zero asks for the display's scale,
        //! corrected by the window's once there is one; a positive value
        //! is used as given and the window does not correct it.
        FTK_UI_API void setDisplayScale(float);

        //! Get the display scale a window draws at: on Auto, the scale of
        //! the display it is on, so a window on a display of another scale
        //! draws for that one; otherwise the scale set.
        FTK_UI_API float getWindowDisplayScale(const IWindow&) const;

        //! Update the display scale from what the system reports for a
        //! window; zero means the platform had no answer. The command
        //! line option still wins.
        FTK_UI_API void setDisplayScaleFromWindow(float);

        FTK_UI_API const std::shared_ptr<CmdLineOption<float> >& getDisplayScaleCmdLineOption() const;

        ///@}

        //! \name Tooltips
        ///@{

        //! Get whether tooltips are shown. On by default, for every window.
        FTK_UI_API bool areTooltipsEnabled() const;
        //! Observe whether tooltips are shown.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeTooltipsEnabled() const;
        FTK_UI_API void setTooltipsEnabled(bool);

        ///@}

        //! \name Offscreen
        ///@{

        //! Get whether windows are drawn without being shown.
        FTK_UI_API bool isOffscreen() const;

        //! Set whether windows are drawn without being shown, applying it to
        //! the windows already added as well as later ones.
        //!
        //! Set by the -screenshot option; an application that takes its own
        //! screenshots sets it for those runs. See IWindow::setOffscreen().
        FTK_UI_API void setOffscreen(bool);

        //! Set whether applications start offscreen, before any of them
        //! exist.
        //!
        //! For test runners and batch tools. An application that is not there
        //! to be looked at should not be putting windows over whatever the
        //! person at the keyboard is doing -- and a window that can be
        //! clicked while something drives it is a run that can be interfered
        //! with, which is worse than untidy.
        //!
        //! Static because the decision belongs to whoever started the process
        //! rather than to any one application in it, and a test suite makes
        //! dozens.
        FTK_UI_API static void setOffscreenDefault(bool);

        ///@}

        //! Observe the monitors.
        FTK_UI_API std::shared_ptr<IObservableList<MonitorInfo> > observeMonitors() const;

        //! \name Suspension
        ///@{

        //! Get whether the system has put the application in the
        //! background, or is ending it: a phone or a tablet does that in
        //! place of a quit, and may not give the application the
        //! processor again. The settings are saved as it happens; an
        //! application saves what else it keeps when this becomes true.
        FTK_UI_API bool isSuspended() const;
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeSuspended() const;

        ///@}

        //! Get the windows.
        FTK_UI_API const std::list<std::shared_ptr<IWindow> >& getWindows() const;

        //! Set the window playback is paced to (see setDisplayPaced()): the
        //! one showing the picture, whose display's refreshes the frames
        //! are chosen for. Nothing for the first window on screen.
        FTK_UI_API void setPacedWindow(const std::shared_ptr<IWindow>&);

        //! A clock the frames are for other than a window's display: a
        //! video output device, which takes a frame each tick of its own
        //! clock. Given the time now, it gives the time of the next tick
        //! and the period in seconds, or false while it has none.
        using FrameTimeSource = std::function<bool(
            const std::chrono::steady_clock::time_point& now,
            std::chrono::steady_clock::time_point& frameTime,
            double& period)>;

        //! Set the clock the frames are for, while a system is busy (a
        //! player playing), in place of the paced window's display
        //! (Context::setFrameTime()); nothing for the window's again. It
        //! applies whether or not playback is paced to the display: the
        //! windows show the same frames, chosen for the other clock.
        FTK_UI_API void setFrameTimeSource(const FrameTimeSource&);

        //! Exit the application.
        FTK_UI_API virtual void exit();

        //! Run the application.
        FTK_UI_API virtual void run();

        //! Tick the event loop. If this method is overridden the base method
        //! should be called.
        FTK_UI_API virtual void tick();

    protected:
        void _addWindow(const std::shared_ptr<IWindow>&);
        void _removeWindow(const std::shared_ptr<IWindow>&);

        //! Create the capture for a -captureShot run. An application
        //! overrides this to supply its Capture subclass.
        FTK_UI_API virtual std::shared_ptr<Capture> _createCapture(
            const std::filesystem::path& manifest,
            const std::string& shotId,
            const std::filesystem::path& outputDir);

    private:
        void _suspend(bool);
        void _runIteration();
        void _screenshotInit(const std::string& fileName);
        void _widgetDumpInit(const std::string& fileName);

        std::shared_ptr<IWindow> _getWindow(uint32_t id) const;
        std::shared_ptr<Window> _pacedWindow() const;
        void _windowResized(const std::shared_ptr<IWindow>&, uint32_t id);
#if defined(FTK_SDL3)
        void _liveResize(uint32_t id);
        static bool _eventWatch(void*, SDL_Event*);
#endif // FTK_SDL3

        void _tickRecursive(
            const std::shared_ptr<IWidget>&,
            bool visible,
            bool enabled,
            const TickEvent&);

        void _displayScaleUpdate(float);
        void _monitorsUpdate();
        void _styleUpdate();

        void _log();

        friend class IWindow;

        FTK_PRIVATE();
    };
}
