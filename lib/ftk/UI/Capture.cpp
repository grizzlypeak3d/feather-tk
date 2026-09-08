// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Capture.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/IButton.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/ScreenshotTag.h>
#include <ftk/UI/TabWidget.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Path.h>
#include <ftk/Core/String.h>
#include <ftk/Core/Timer.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>

namespace ftk
{
    namespace
    {
        // Budgets in ticks of the timer below, which fires on the clock
        // rather than once per drawn frame -- so these are durations, and
        // the machine being fast or slow does not change them.
        const std::chrono::milliseconds tickInterval(30);
        const int settleTicks = 15;     // 450ms to settle before capturing
        const int reloadGraceTicks = 4; // 120ms for a reload to begin
        const int timeoutTicks = 400;   // 12s hard cap waiting for media

        void collect(
            const std::shared_ptr<IWidget>& widget,
            std::vector<std::shared_ptr<IWidget> >& out)
        {
            if (!widget)
                return;
            if (hasScreenshotTag(widget) && widget->isVisible(true))
                out.push_back(widget);
            for (const auto& child : widget->getChildren())
                collect(child, out);
        }

        //! The text a widget is showing, for the kinds that show any. A
        //! tagged widget is often a row or a container rather than the
        //! label itself, so this looks down the tree and joins what it
        //! finds: the point is to make what is on screen readable from the
        //! sidecar instead of by cropping the image and looking at it.
        std::string widgetText(const std::shared_ptr<IWidget>& widget)
        {
            std::vector<std::string> out;
            // Only what is on screen. A hidden child still has its text,
            // and reporting it would say the capture shows something it
            // does not -- which is the one thing this must not do.
            if (!widget->isVisible(true))
            {
                return std::string();
            }
            if (auto label = std::dynamic_pointer_cast<Label>(widget))
            {
                out.push_back(label->getText());
            }
            else if (auto lineEdit = std::dynamic_pointer_cast<LineEdit>(widget))
            {
                out.push_back(lineEdit->getText());
            }
            else if (auto comboBox = std::dynamic_pointer_cast<ComboBox>(widget))
            {
                const auto& items = comboBox->getItems();
                const int i = comboBox->getCurrentIndex();
                if (i >= 0 && i < static_cast<int>(items.size()))
                {
                    out.push_back(items[i].text);
                }
            }
            else if (auto button = std::dynamic_pointer_cast<IButton>(widget))
            {
                // A button reads as its label plus its state, since the
                // state is usually the thing under test. Most of the tool
                // bar has an icon and no label, and "[unchecked]" on its own
                // says nothing about which button it was, so fall back to
                // the icon name.
                std::string s = button->getText();
                if (s.empty())
                {
                    s = button->getIcon();
                }
                if (button->isCheckable())
                {
                    s += button->isChecked() ? " [checked]" : " [unchecked]";
                }
                out.push_back(s);
            }
            else
            {
                for (const auto& child : widget->getChildren())
                {
                    const std::string s = widgetText(child);
                    if (!s.empty())
                    {
                        out.push_back(s);
                    }
                }
            }
            out.erase(
                std::remove_if(
                    out.begin(),
                    out.end(),
                    [](const std::string& value) { return value.empty(); }),
                out.end());
            return join(out, " ");
        }

        std::shared_ptr<TabWidget> findTabWidget(
            const std::shared_ptr<IWidget>& widget,
            const std::string& tab)
        {
            if (auto tabWidget = std::dynamic_pointer_cast<TabWidget>(widget))
            {
                const auto& tabs = tabWidget->getTabs();
                if (std::find(tabs.begin(), tabs.end(), tab) != tabs.end())
                    return tabWidget;
            }
            for (const auto& child : widget->getChildren())
            {
                if (auto found = findTabWidget(child, tab))
                    return found;
            }
            return nullptr;
        }

        int parseModifiers(const nlohmann::json& step)
        {
            int out = 0;
            if (step.contains("modifier"))
            {
                KeyModifier modifier = KeyModifier::None;
                if (from_string(
                    step.at("modifier").get<std::string>(), modifier))
                {
                    out = static_cast<int>(modifier);
                }
            }
            return out;
        }

        enum class Phase { WaitReady, ApplyRest, Reload, Settle, Done };
    }

    struct Capture::Private
    {
        std::weak_ptr<Context> context;
        std::weak_ptr<App> app;
        std::string appName;
        std::filesystem::path manifest;
        std::string shotId;
        std::filesystem::path outputDir;

        nlohmann::json shot;
        bool expectMedia = false;

        std::shared_ptr<Timer> timer;
        Phase phase = Phase::WaitReady;
        int ticks = 0;
        int settleLeft = settleTicks;
        int settleTicksShot = settleTicks; // per-shot, from the "settle" field
        int reloadGrace = reloadGraceTicks;
        std::vector<nlohmann::json> lateSteps;  // applied after first settle
        size_t lateNext = 0;                   // next late step to apply
        int waitTicks = 0;                     // from the "wait" step
        bool done = false;
        bool success = false;
    };

    void Capture::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::filesystem::path& manifest,
        const std::string& shotId,
        const std::filesystem::path& outputDir)
    {
        FTK_P();
        p.context = context;
        p.app = app;
        p.appName = app->getName();
        p.manifest = manifest;
        p.shotId = shotId;
        p.outputDir = outputDir;
    }

    Capture::Capture() :
        _p(new Private)
    {}

    Capture::~Capture()
    {}

    std::shared_ptr<Capture> Capture::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::filesystem::path& manifest,
        const std::string& shotId,
        const std::filesystem::path& outputDir)
    {
        auto out = std::shared_ptr<Capture>(new Capture);
        out->_init(context, app, manifest, shotId, outputDir);
        return out;
    }

    bool Capture::begin()
    {
        FTK_P();
        auto context = p.context.lock();
        auto app = p.app.lock();
        if (!context || !app)
            return false;

        // Resolve the requested shot from the manifest.
        try
        {
            std::ifstream f(p.manifest);
            if (!f.is_open())
                throw std::runtime_error(Format(
                    "cannot open manifest \"{0}\"").arg(fromFileSystem(p.manifest)));
            nlohmann::json doc;
            f >> doc;
            bool found = false;
            for (const auto& shot : doc.at("shots"))
            {
                if (shot.value("id", std::string()) == p.shotId)
                {
                    p.shot = shot;
                    found = true;
                    break;
                }
            }
            if (!found)
                throw std::runtime_error("shot id not found in manifest");
        }
        catch (const std::exception& e)
        {
            _note(e.what());
            return false;
        }

        // A shot may widen the settle window (in seconds) to let slow async
        // work finish before the capture -- e.g. timeline thumbnails, which
        // stream in after the media is ready. Defaults to settleTicks.
        const double settleSeconds = p.shot.value("settle", 0.0);
        if (settleSeconds > 0.0)
        {
            int ticks = static_cast<int>(
                settleSeconds * 1000.0 / tickInterval.count());
            if (ticks < settleTicks)
                ticks = settleTicks;
            p.settleTicksShot = ticks;
        }

        if (app->getWindows().empty())
        {
            _note("no window was created");
            return false;
        }
        auto window = app->getWindows().front();

        // Deterministic presentation. Capture runs should also pass
        // -resetSettings so saved window state can't override this.
        app->setColorStyle(ColorStyle::Dark);
        app->setTooltipsEnabled(false);
        if (p.shot.contains("window"))
        {
            const auto& w = p.shot.at("window");
            if (w.contains("w") && w.contains("h"))
                window->setSize(Size2I(w.at("w").get<int>(), w.at("h").get<int>()));
            if (w.contains("scale"))
                app->setDisplayScale(w.at("scale").get<float>());
            _setupWindow(w);
        }
        // Nobody is watching a capture run, and a window on screen can be
        // clicked or hovered while the shot is being taken, which puts a
        // highlight or a tooltip into it.
        app->setOffscreen(true);
        window->show();

        // Apply the early setup steps now; the rest of the setup waits
        // until the content is ready (handled in the timer state machine).
        _applyEarly(p.shot.value("setup", nlohmann::json::array()));

        // Arm the capture timer. It fires from inside App::run(), after
        // the window is realized and drawing.
        p.timer = Timer::create(context);
        p.timer->setRepeating(true);
        auto weak = std::weak_ptr<Capture>(shared_from_this());
        p.timer->start(tickInterval, [weak] {
            if (auto self = weak.lock())
                self->_onTick();
        });
        return true;
    }

    bool Capture::succeeded() const
    {
        return _p->success;
    }

    void Capture::_setupWindow(const nlohmann::json&)
    {}

    void Capture::_applyEarly(const nlohmann::json&)
    {}

    bool Capture::_ready() const
    {
        return true;
    }

    bool Capture::_frameReady() const
    {
        return true;
    }

    std::string Capture::_mediaError() const
    {
        return std::string();
    }

    std::string Capture::_waitingFor() const
    {
        return "the shot to become ready";
    }

    bool Capture::_isEarlyStep(const nlohmann::json&) const
    {
        return false;
    }

    bool Capture::_isLateStep(const nlohmann::json& step) const
    {
        // A click, hover, or drag aims at a widget's laid-out geometry, and
        // a key press needs the focused widget to exist. A tab selection
        // searches the widget tree, so it must wait until whatever a
        // preceding step created has been laid out.
        return
            step.contains("click") ||
            step.contains("scroll") ||
            step.contains("drag") ||
            step.contains("hover") ||
            step.contains("key") ||
            step.contains("tab") ||
            step.contains("wait");
    }

    bool Capture::_applyStep(const nlohmann::json& step)
    {
        FTK_P();
        auto app = p.app.lock();
        if (!app)
            return false;
        std::shared_ptr<IWindow> window;
        if (!app->getWindows().empty())
            window = app->getWindows().front();
        if (step.contains("tab"))
        {
            // Select a tab by name, e.g. { "tab": "Movie" }. Searches the
            // window for a tab widget containing the given tab name.
            // Deferred to the late phase so the tool from a preceding step
            // has been created (see _applyRest).
            const std::string name = step.at("tab").get<std::string>();
            std::shared_ptr<TabWidget> tabWidget;
            if (window)
            {
                tabWidget = findTabWidget(window, name);
            }
            if (tabWidget)
            {
                const auto& tabs = tabWidget->getTabs();
                const auto i = std::find(tabs.begin(), tabs.end(), name);
                tabWidget->setCurrent(i - tabs.begin());
            }
            else
            {
                _note(Format("tab not found: \"{0}\"").arg(name));
            }
        }
        else if (step.contains("wait"))
        {
            // Hold this many seconds before the next step, for the things
            // that happen outside the app -- a render adding frames to a
            // sequence the shot is about to reload. e.g. { "wait": 2.0 }.
            // The wait spends the shot's overall timeout budget.
            p.waitTicks = static_cast<int>(
                step.at("wait").get<double>() * 1000.0 /
                tickInterval.count());
        }
        else if (step.contains("scroll"))
        {
            // Scroll the mouse wheel over a tagged widget or a window
            // position, aimed the way "click" is aimed. e.g.
            // { "scroll": "MainWindow.Viewport", "delta": [0, 1] },
            // { "scroll": [160, 90], "delta": [0, -1], "modifier":
            // "Control" }. Deferred by _applyRest like "click".
            const auto& v = step.at("scroll");
            const int modifiers = parseModifiers(step);
            V2F delta(0.F, 1.F);
            if (step.contains("delta") &&
                step.at("delta").is_array() &&
                step.at("delta").size() >= 2)
            {
                delta.x = step.at("delta")[0].get<float>();
                delta.y = step.at("delta")[1].get<float>();
            }
            std::optional<V2I> pos;
            if (v.is_array() && v.size() >= 2)
            {
                pos = V2I(v[0].get<int>(), v[1].get<int>());
            }
            else if (v.is_string() && window)
            {
                std::vector<std::shared_ptr<IWidget> > tagged;
                collect(window, tagged);
                for (const auto& w : tagged)
                {
                    if (getScreenshotTag(w) == v.get<std::string>())
                    {
                        const Box2I g = w->getGeometry();
                        pos = V2I(
                            g.x() + g.w() / 2,
                            g.y() + g.h() / 2);
                        break;
                    }
                }
                if (!pos.has_value())
                {
                    _note(
                        "scroll: no visible widget tagged \"" +
                        v.get<std::string>() + "\"");
                }
            }
            if (pos.has_value() && window)
            {
                window->scroll(pos.value(), delta, modifiers);
            }
        }
        else if (step.contains("click"))
        {
            // Click on the widget with a screenshot tag, or at a window
            // position in framebuffer pixels -- on a display with a scale
            // of two the numbers are twice the requested window size, so
            // the tag form is preferred where a tag exists. The button
            // defaults to the left, e.g.
            // { "click": "Files.CompareMode" },
            // { "click": "MainWindow.Viewport", "button": "Right" },
            // { "click": [160, 90], "modifier": "Ctrl" }. This goes through
            // the window the way a real click does -- including the mouse
            // bindings, so an action needs the modifier it is bound to.
            // Deferred by _applyRest so the widget under it is laid out.
            const auto& v = step.at("click");
            const int modifiers = parseModifiers(step);
            MouseButton button = MouseButton::Left;
            if (step.contains("button"))
            {
                from_string(
                    step.at("button").get<std::string>(), button);
            }
            std::optional<V2I> pos;
            if (v.is_array() && v.size() >= 2)
            {
                pos = V2I(v[0].get<int>(), v[1].get<int>());
            }
            else if (v.is_string() && window)
            {
                std::vector<std::shared_ptr<IWidget> > tagged;
                collect(window, tagged);
                for (const auto& w : tagged)
                {
                    if (getScreenshotTag(w) == v.get<std::string>())
                    {
                        const Box2I g = w->getGeometry();
                        pos = V2I(
                            g.x() + g.w() / 2,
                            g.y() + g.h() / 2);
                        break;
                    }
                }
                if (!pos.has_value())
                {
                    _note(
                        "click: no visible widget tagged \"" +
                        v.get<std::string>() + "\"");
                }
            }
            if (pos.has_value() && window)
            {
                window->click(pos.value(), button, modifiers);
            }
        }
        else if (step.contains("hover"))
        {
            // Move the cursor without pressing, aimed the way "click"
            // is aimed, for what only shows on hover -- the status
            // bar's menu hints, say. Deferred by _applyRest like
            // "click", e.g.
            // { "hover": "Files.CompareMode" },
            // { "hover": [160, 90] }.
            const auto& v = step.at("hover");
            std::optional<V2I> pos;
            if (v.is_array() && v.size() >= 2)
            {
                pos = V2I(v[0].get<int>(), v[1].get<int>());
            }
            else if (v.is_string() && window)
            {
                std::vector<std::shared_ptr<IWidget> > tagged;
                collect(window, tagged);
                for (const auto& w : tagged)
                {
                    if (getScreenshotTag(w) == v.get<std::string>())
                    {
                        const Box2I g = w->getGeometry();
                        pos = V2I(
                            g.x() + g.w() / 2,
                            g.y() + g.h() / 2);
                        break;
                    }
                }
                if (!pos.has_value())
                {
                    _note(
                        "hover: no visible widget tagged \"" +
                        v.get<std::string>() + "\"");
                }
            }
            if (pos.has_value() && window)
            {
                window->hover(pos.value());
            }
        }
        else if (step.contains("drag"))
        {
            // Press at the first point, move through the rest, release --
            // the window's drag helper, aimed the way "click" is aimed.
            // Each entry is a screenshot tag, whose widget's center is
            // used, or a position in framebuffer pixels, e.g.
            // { "drag": ["Files.Thumbnail", [160, 300]] },
            // { "drag": [[100, 50], [100, 250]], "modifier": "Ctrl" }.
            // Deferred by _applyRest like "click".
            const auto& v = step.at("drag");
            const int modifiers = parseModifiers(step);
            std::vector<V2I> path;
            bool pathOK = v.is_array() && window;
            if (pathOK)
            {
                std::vector<std::shared_ptr<IWidget> > tagged;
                collect(window, tagged);
                for (const auto& entry : v)
                {
                    if (entry.is_array() && entry.size() >= 2)
                    {
                        path.push_back(V2I(
                            entry[0].get<int>(),
                            entry[1].get<int>()));
                    }
                    else if (entry.is_string())
                    {
                        bool found = false;
                        for (const auto& w : tagged)
                        {
                            if (getScreenshotTag(w) ==
                                entry.get<std::string>())
                            {
                                const Box2I g = w->getGeometry();
                                path.push_back(V2I(
                                    g.x() + g.w() / 2,
                                    g.y() + g.h() / 2));
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            pathOK = false;
                            _note(
                                "drag: no visible widget tagged \"" +
                                entry.get<std::string>() + "\"");
                        }
                    }
                }
            }
            if (pathOK && path.size() >= 2)
            {
                window->drag(path, modifiers);
            }
        }
        else if (step.contains("text"))
        {
            // Type text, the way typing does after the key events:
            // letters reach a widget as text input, not as key
            // presses. e.g. { "text": "-0.50" }. Deferred like "click",
            // so the field it lands in is laid out and focused.
            if (window)
            {
                window->text(step.at("text").get<std::string>());
            }
        }
        else if (step.contains("key"))
        {
            // Press and release a key through the window's dispatch,
            // e.g. { "key": "Escape" }, { "key": "M", "modifier":
            // "Shift" }. Deferred like "click" so the focused widget
            // exists.
            Key key = Key::Unknown;
            if (from_string(step.at("key").get<std::string>(), key) &&
                Key::Unknown != key)
            {
                if (window)
                {
                    window->keyPress(key, parseModifiers(step));
                }
            }
            else
            {
                _note(
                    "key: unknown key \"" +
                    step.at("key").get<std::string>() + "\"");
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    std::shared_ptr<Context> Capture::_getContext() const
    {
        return _p->context.lock();
    }

    std::shared_ptr<App> Capture::_getApp() const
    {
        return _p->app.lock();
    }

    const std::string& Capture::_getShotId() const
    {
        return _p->shotId;
    }

    const nlohmann::json& Capture::_getShot() const
    {
        return _p->shot;
    }

    void Capture::_setExpectMedia(bool value)
    {
        _p->expectMedia = value;
    }

    void Capture::_addLateStep(const nlohmann::json& step)
    {
        _p->lateSteps.push_back(step);
    }

    void Capture::_note(const std::string& msg) const
    {
        FTK_P();
        std::cerr << p.appName << " capture [" << p.shotId << "]: " <<
            msg << std::endl;
    }

    void Capture::_onTick()
    {
        FTK_P();
        if (p.done)
            return;
        ++p.ticks;
        if (p.ticks > timeoutTicks + (p.settleTicksShot - settleTicks))
        {
            _note(Format("timed out waiting for {0}").
                arg(p.expectMedia ?
                    _waitingFor() :
                    std::string("the shot to become ready")));
            _finish(false);
            return;
        }

        switch (p.phase)
        {
        case Phase::WaitReady:
            if (!p.expectMedia || _ready())
            {
                p.phase = Phase::ApplyRest;
            }
            else
            {
                const std::string error = _mediaError();
                if (!error.empty())
                {
                    _note(Format("cannot read the media: {0}").
                        arg(error));
                    _finish(false);
                    return;
                }
            }
            break;
        case Phase::ApplyRest:
            _applyRest(p.shot.value("setup", nlohmann::json::array()));
            p.phase = Phase::Reload;
            p.reloadGrace = reloadGraceTicks;
            break;
        case Phase::Reload:
            // Setup steps that change the content (A/B, compare) make it
            // reload. Give the reload a moment to begin, then wait until
            // the content is ready again before settling.
            if (p.reloadGrace > 0)
                --p.reloadGrace;
            else if (!p.expectMedia || (_ready() && _frameReady()))
            {
                p.phase = Phase::Settle;
                p.settleLeft = p.settleTicksShot;
            }
            break;
        case Phase::Settle:
            if (--p.settleLeft <= 0)
            {
                if (p.lateNext < p.lateSteps.size())
                {
                    // The window is now sized and laid out; apply the
                    // deferred steps one per settle rather than in one go,
                    // so each one is drawn before the next runs. A step that
                    // reads the rendered image needs the step before it to
                    // have reached the screen first -- and a shot that reads
                    // and then changes the image is testing what happens
                    // when the image changes underneath the reader, which
                    // cannot happen if both land in the same frame.
                    //
                    // Copied because applying a step can append another.
                    const nlohmann::json step = p.lateSteps[p.lateNext++];
                    if (!_applyStep(step))
                    {
                        _note("unknown step: " + step.dump());
                    }
                    p.settleLeft = std::max(p.settleTicksShot, p.waitTicks);
                    p.waitTicks = 0;
                }
                else
                {
                    p.phase = Phase::Done; // captured below this switch
                }
            }
            break;
        default:
            break;
        }

        if (Phase::Done == p.phase && !p.done)
        {
            std::error_code ec;
            std::filesystem::create_directories(p.outputDir, ec);
            const auto png = p.outputDir / (p.shotId + ".png");
            const auto json = p.outputDir / (p.shotId + ".json");
            bool ok = _writePNG(png);
            if (ok)
            {
                _writeMetadata(json);
                _note(Format("captured {0}").arg(fromFileSystem(png)));
            }
            _finish(ok);
        }
    }

    void Capture::_applyRest(const nlohmann::json& setup)
    {
        FTK_P();
        // Everything after the first deferred step is deferred too. Only
        // some steps have to wait, but applying the rest immediately would
        // run them before the ones that waited, quietly reordering the
        // manifest -- a shot that reads the image and then changes it would
        // become one that changes and then reads, which samples the settled
        // result and can never catch a stale one.
        bool late = false;
        for (const auto& step : setup)
        {
            if (_isEarlyStep(step))
                continue;
            late = late || _isLateStep(step);
            if (late)
            {
                p.lateSteps.push_back(step);
                continue;
            }
            if (!_applyStep(step))
            {
                _note("unknown step: " + step.dump());
            }
        }
    }

    void Capture::_finish(bool ok)
    {
        FTK_P();
        p.success = ok;
        p.done = true;
        if (p.timer)
            p.timer->stop();
        if (auto app = p.app.lock())
            app->exit();
    }

    bool Capture::_writePNG(const std::filesystem::path& path) const
    {
        FTK_P();
        auto app = p.app.lock();
        if (!app)
            return false;
        if (!app->writeScreenshot(path))
        {
            _note(Format("cannot capture \"{0}\"").arg(fromFileSystem(path)));
            return false;
        }
        return true;
    }

    void Capture::_writeMetadata(const std::filesystem::path& path) const
    {
        FTK_P();
        auto app = p.app.lock();
        if (!app || app->getWindows().empty())
            return;
        auto window = app->getWindows().front();

        std::vector<std::shared_ptr<IWidget> > tagged;
        collect(window, tagged);

        nlohmann::json widgets = nlohmann::json::array();
        for (const auto& w : tagged)
        {
            const Box2I g = w->getGeometry();
            nlohmann::json widget = {
                { "id", getScreenshotTag(w) },
                { "box", { g.x(), g.y(), g.w(), g.h() } } };
            const std::string text = widgetText(w);
            if (!text.empty())
            {
                widget["text"] = text;
            }
            widgets.push_back(widget);
        }

        // Boxes and the screenshot share the offscreen buffer's pixel space
        // (window size x display scale), so they already line up regardless
        // of scale. Record the display scale as "dpr" so make_svg can show a
        // high-DPI capture at its logical size -- crisp, not enlarged.
        const Size2I size = window->getGeometry().size();
        nlohmann::json out = {
            { "shot", p.shotId },
            { "image", p.shotId + ".png" },
            { "dpr", app->getDisplayScale() },
            { "window", { { "w", size.w }, { "h", size.h } } },
            { "widgets", widgets } };
        if (p.shot.contains("annotate"))
            out["annotate"] = p.shot.at("annotate");
        // Per-shot make_svg options pass straight through to the sidecar so
        // the SVG step is driven entirely by the sidecar -- build_screenshots
        // never has to re-parse the manifest to forward them.
        if (p.shot.contains("crop"))
            out["crop"] = p.shot.at("crop");
        if (p.shot.contains("layout"))
            out["layout"] = p.shot.at("layout");
        if (p.shot.contains("cropFit"))
            out["cropFit"] = p.shot.at("cropFit");

        std::ofstream f(path);
        f << out.dump(2) << std::endl;
    }
}
