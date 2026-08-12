// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Style.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/IApp.h>
#include <ftk/Core/ObservableList.h>
#include <ftk/Core/ObservableMap.h>
#include <ftk/Core/Observable.h>

#include <filesystem>
#include <list>

namespace ftk
{
    class FontSystem;
    class Settings;
    class IWidget;
    class IWindow;
    class IconSystem;
    struct TickEvent;

    //! Monitor information.
    struct FTK_API_TYPE MonitorInfo
    {
        std::string name;
        Size2I      size;
        int         refreshRate = 0;
        V2F         dpi;
        Box2I       bounds;

        bool operator == (const MonitorInfo&) const;
        bool operator != (const MonitorInfo&) const;
    };

    //! Color style.
    enum class FTK_API_TYPE ColorStyle
    {
        Dark,
        Light,
        Custom,

        Count,
        First = Dark
    };
    FTK_ENUM(ColorStyle);

    //! Get custom color roles.
    FTK_API std::map<ColorRole, Color4F> getCustomColorRoles();

    //! Get display scales.
    FTK_API std::vector<float> getDisplayScales();

    //! Where an application keeps its per-user files: the settings and the
    //! log. Left empty, no files are kept and the application is responsible
    //! for its own, which is what applications did before this existed.
    //!
    //! A suite of applications gives them all the same directory and tells
    //! them apart by the base name, so that one directory holds the settings
    //! and logs of all of them rather than one directory apiece.
    struct FTK_API_TYPE AppFiles
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
    class FTK_API_TYPE App : public IApp
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {},
            const AppFiles& = AppFiles());

        App();

    public:
        FTK_API virtual ~App();

        //! Create a new application.
        FTK_API static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            int argc, char** argv,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Create a new application.
        FTK_API static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            int argc, wchar_t* argv[],
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Create a new application.
        FTK_API static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Get the font system.
        FTK_API const std::shared_ptr<FontSystem>& getFontSystem() const;

        //! Get the icon system.
        FTK_API const std::shared_ptr<IconSystem>& getIconSystem() const;

        //! Get the style.
        FTK_API const std::shared_ptr<Style>& getStyle() const;

        //! \name Files
        ///@{

        //! Get the settings. Null when no AppFiles was given.
        FTK_API const std::shared_ptr<Settings>& getSettings() const;

        //! Get the settings file path. Empty when no AppFiles was given.
        FTK_API const std::filesystem::path& getSettingsPath() const;

        //! Get the log file path. Empty when no AppFiles was given.
        FTK_API const std::filesystem::path& getLogFilePath() const;

        //! Write a screenshot of the first window. Returns false when there
        //! is no window, when the offscreen buffer is not ready, or when the
        //! file cannot be written; the reason is logged.
        //!
        //! Only useful once the event loop has run: that is what realizes and
        //! sizes the window and leaves a buffer behind to read.
        FTK_API bool writeScreenshot(const std::filesystem::path&);

        ///@}

        //! \name Color Style
        ///@{

        FTK_API ColorStyle getColorStyle() const;
        FTK_API std::shared_ptr<IObservable<ColorStyle> > observeColorStyle() const;
        FTK_API void setColorStyle(ColorStyle);

        FTK_API const std::map<ColorRole, Color4F>& getCustomColorRoles() const;
        FTK_API std::shared_ptr<IObservableMap<ColorRole, Color4F> > observeCustomColorRoles() const;
        FTK_API void setCustomColorRoles(const std::map<ColorRole, Color4F>&);

        FTK_API const std::shared_ptr<CmdLineOption<ColorStyle> >& getColorStyleCmdLineOption() const;

        ///@}

        //! \name Display Scale
        ///@{

        FTK_API float getDefaultDisplayScale() const;
        FTK_API float getDisplayScale() const;
        FTK_API std::shared_ptr<IObservable<float> > observeDisplayScale() const;
        FTK_API void setDisplayScale(float);

        FTK_API const std::shared_ptr<CmdLineOption<float> >& getDisplayScaleCmdLineOption() const;

        ///@}

        //! \name Tooltips
        ///@{

        FTK_API bool areTooltipsEnabled() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeTooltipsEnabled() const;
        FTK_API void setTooltipsEnabled(bool);

        ///@}

        //! \name Offscreen
        ///@{

        //! Get whether windows are drawn without being shown.
        FTK_API bool isOffscreen() const;

        //! Set whether windows are drawn without being shown, applying it to
        //! the windows already added as well as later ones.
        //!
        //! Set by the -screenshot option; an application that takes its own
        //! screenshots sets it for those runs. See IWindow::setOffscreen().
        FTK_API void setOffscreen(bool);

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
        FTK_API static void setOffscreenDefault(bool);

        ///@}

        //! Observe the monitors.
        FTK_API std::shared_ptr<IObservableList<MonitorInfo> > observeMonitors() const;

        //! Get the windows.
        FTK_API const std::list<std::shared_ptr<IWindow> >& getWindows() const;

        //! Exit the application.
        FTK_API virtual void exit();

        //! Run the application.
        FTK_API virtual void run();

        //! Tick the event loop. If this method is overridden the base method
        //! should be called.
        FTK_API virtual void tick();

    protected:
        void _addWindow(const std::shared_ptr<IWindow>&);
        void _removeWindow(const std::shared_ptr<IWindow>&);

    private:
        void _screenshotInit(const std::string& fileName);

        std::shared_ptr<IWindow> _getWindow(uint32_t id) const;

        void _tickRecursive(
            const std::shared_ptr<IWidget>&,
            bool visible,
            bool enabled,
            const TickEvent&);

        void _monitorsUpdate();
        void _styleUpdate();

        void _log();

        friend class IWindow;

        FTK_PRIVATE();
    };
}
