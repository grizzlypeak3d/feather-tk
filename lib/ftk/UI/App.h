// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Style.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/IApp.h>
#include <ftk/Core/ObservableList.h>
#include <ftk/Core/ObservableMap.h>
#include <ftk/Core/ObservableValue.h>

#include <list>

namespace ftk
{
    class FontSystem;
    class IWidget;
    class IWindow;
    class IconSystem;
    class TickEvent;

    //! Monitor information.
    struct MonitorInfo
    {
        std::string name;
        Size2I      size;
        int         refreshRate = 0;
        float       dDpi = 0.F;
        float       hDpi = 0.F;
        float       vDpi = 0.F;
        Box2I       bounds;

        bool operator == (const MonitorInfo&) const;
        bool operator != (const MonitorInfo&) const;
    };

    //! Color style.
    enum ColorStyle
    {
        Dark,
        Light,
        Custom,

        Count,
        First = Dark
    };
    FTK_ENUM(ColorStyle);

    //! Get custom color roles.
    std::map<ColorRole, Color4F> getCustomColorRoles();

    //! Get display scales.
    std::vector<float> getDisplayScales();

    //! Base class for user interface applications.
    class App : public IApp
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
        virtual ~App();

        //! Create a new application.
        static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            int argc, char** argv,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Create a new application.
        static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            int argc, wchar_t* argv[],
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Create a new application.
        static std::shared_ptr<App> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& = {},
            const std::vector<std::shared_ptr<ICmdLineOption> >& = {});

        //! Get the windows.
        const std::list<std::shared_ptr<IWindow> >& getWindows() const;

        //! Observe the monitors.
        std::shared_ptr<IObservableList<MonitorInfo> > observeMonitors() const;

        //! Get the font system.
        const std::shared_ptr<FontSystem>& getFontSystem() const;

        //! Get the icon system.
        const std::shared_ptr<IconSystem>& getIconSystem() const;

        //! Get the style.
        const std::shared_ptr<Style>& getStyle() const;

        //! Get the color style.
        ColorStyle getColorStyle() const;

        //! Observe the color style.
        std::shared_ptr<IObservableValue<ColorStyle> > observeColorStyle() const;

        //! Set the color style.
        void setColorStyle(ColorStyle);

        //! Get the custom color roles.
        const std::map<ColorRole, Color4F>& getCustomColorRoles() const;

        //! Observe the custom color roles.
        std::shared_ptr<IObservableMap<ColorRole, Color4F> > observeCustomColorRoles() const;

        //! Set the custom color roles.
        void setCustomColorRoles(const std::map<ColorRole, Color4F>&);

        //! Get the color style command line option.
        const std::shared_ptr<CmdLineValueOption<ColorStyle> >& getColorStyleCmdLineOption() const;

        //! Get the default display scale.
        float getDefaultDisplayScale() const;

        //! Get the display scale.
        float getDisplayScale() const;

        //! Observe the display scale.
        std::shared_ptr<IObservableValue<float> > observeDisplayScale() const;

        //! Set the display scale.
        void setDisplayScale(float);

        //! Get the display scale command line option.
        const std::shared_ptr<CmdLineValueOption<float> >& getDisplayScaleCmdLineOption() const;

        //! Get whether tooltips are enabled.
        bool areTooltipsEnabled() const;

        //! Observe whether tooltips are enabled.
        std::shared_ptr<IObservableValue<bool> > observeTooltipsEnabled() const;

        //! Set whether tooltips are enabled.
        void setTooltipsEnabled(bool);

        //! Exit the application.
        virtual void exit();

        //! Tick the event loop. If this method is overridden the base method
        //! should be called.
        virtual void tick();

        void run() override;

    protected:
        void _addWindow(const std::shared_ptr<IWindow>&);
        void _removeWindow(const std::shared_ptr<IWindow>&);

    private:
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
