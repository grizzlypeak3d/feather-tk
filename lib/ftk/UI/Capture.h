// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>

#include <ftk/Core/Util.h>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace ftk
{
    class App;
    class Context;

    //! Automated screenshot capture, for documentation and tests.
    //!
    //! A JSON manifest lists "shots", each with an id, optional window
    //! setup, and a list of setup steps to apply before the capture --
    //! clicks, key presses, typed text, scrolling, dragging, hovering, tab
    //! selection, and waits. The steps aim at widgets by their screenshot
    //! tag (see ScreenshotTag.h). The capture writes a PNG and a JSON
    //! sidecar recording the tagged widgets' geometry and text.
    //!
    //! One shot per process, requested with the -captureShot option (see
    //! App). Capture is driven from a timer running inside the normal event
    //! loop (App::run()), which is what realizes and sizes the window and
    //! produces a valid offscreen buffer to read back.
    //!
    //! An application subclasses this to add steps of its own and to tell
    //! the capture when its content is ready; see the protected virtuals.
    class FTK_UI_API_TYPE Capture : public std::enable_shared_from_this<Capture>
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::filesystem::path& manifest,
            const std::string& shotId,
            const std::filesystem::path& outputDir);

        FTK_UI_API Capture();

    public:
        FTK_UI_API virtual ~Capture();

        FTK_UI_API static std::shared_ptr<Capture> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::filesystem::path& manifest,
            const std::string& shotId,
            const std::filesystem::path& outputDir);

        //! Parse the manifest, set up deterministic window state, apply the
        //! early setup steps, and arm the capture timer. Returns false on a
        //! setup error (bad manifest, unknown shot, no window). After this
        //! returns true, the caller runs the event loop.
        FTK_UI_API bool begin();

        //! Whether the capture completed and wrote its outputs.
        FTK_UI_API bool succeeded() const;

    protected:
        //! \name Hooks
        //! Overridden by an application's subclass, listed in the order
        //! they run.
        ///@{

        //! Extra window setup from the shot's "window" object, after the
        //! size and scale are applied -- splitter positions, say.
        FTK_UI_API virtual void _setupWindow(const nlohmann::json& window);

        //! Setup steps that must run before the event loop starts -- steps
        //! that load the content the rest of the shot works on. A subclass
        //! that loads something here calls _setExpectMedia() so the capture
        //! waits for _ready() before applying the remaining steps.
        FTK_UI_API virtual void _applyEarly(const nlohmann::json& setup);

        //! Whether the content opened by _applyEarly() is ready for the
        //! remaining setup steps.
        FTK_UI_API virtual bool _ready() const;

        //! Whether the content is drawn and settled after the setup steps --
        //! e.g. an asynchronously decoded frame has arrived.
        FTK_UI_API virtual bool _frameReady() const;

        //! Why the content can never become ready, or an empty string while
        //! it still might. A non-empty string fails the shot at once instead
        //! of waiting out the timeout.
        FTK_UI_API virtual std::string _mediaError() const;

        //! What the shot is waiting for, for the timeout message.
        FTK_UI_API virtual std::string _waitingFor() const;

        //! Whether a step was already consumed by _applyEarly(), so the
        //! state machine skips it.
        FTK_UI_API virtual bool _isEarlyStep(const nlohmann::json& step) const;

        //! Whether a step must wait until the window is laid out and
        //! settled. A subclass ORs its own late steps with the base's.
        FTK_UI_API virtual bool _isLateStep(const nlohmann::json& step) const;

        //! Apply one setup step, returning whether the step was recognized.
        //! A subclass tries its own steps and hands the rest to the base.
        FTK_UI_API virtual bool _applyStep(const nlohmann::json& step);

        ///@}

        //! \name Subclass services
        ///@{

        FTK_UI_API std::shared_ptr<Context> _getContext() const;
        FTK_UI_API std::shared_ptr<App> _getApp() const;
        FTK_UI_API const std::string& _getShotId() const;
        FTK_UI_API const nlohmann::json& _getShot() const;

        //! Wait for _ready() before applying the remaining setup steps;
        //! see _applyEarly().
        FTK_UI_API void _setExpectMedia(bool);

        //! Queue a step to run in the late phase, after the window is laid
        //! out and settled.
        FTK_UI_API void _addLateStep(const nlohmann::json& step);

        //! Console diagnostics so failures are not silent.
        FTK_UI_API void _note(const std::string&) const;

        ///@}

    private:
        void _onTick();
        void _applyRest(const nlohmann::json& setup);
        void _finish(bool ok);

        bool _writePNG(const std::filesystem::path&) const;
        void _writeMetadata(const std::filesystem::path&) const;

        FTK_PRIVATE();
    };
}
