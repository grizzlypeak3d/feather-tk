// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Style.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/IApp.h>
#include <ftk/Core/ObservableList.h>
#include <ftk/Core/ObservableMap.h>
#include <ftk/Core/Observable.h>

#include <list>

namespace ftk
{
    class FontSystem;
    class IWidget;
    class IWindow;
    class IconSystem;
    class TickEvent;

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
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

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

        //! Observe the monitors.
        FTK_API std::shared_ptr<IObservableList<MonitorInfo> > observeMonitors() const;

        //! Get the windows.
        FTK_API const std::list<std::shared_ptr<IWindow> >& getWindows() const;

        //! Exit the application.
        FTK_API virtual void exit();

        //! Tick the event loop. If this method is overridden the base method
        //! should be called.
        FTK_API virtual void tick();

        FTK_API void run() override;

    protected:
        void _addWindow(const std::shared_ptr<IWindow>&);
        void _removeWindow(const std::shared_ptr<IWindow>&);

    private:
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
