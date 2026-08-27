// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ClipboardSystem.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>

namespace ftk
{
    namespace
    {
        ClipboardSystem* clipboardSystemInstance = nullptr;
    }
}

extern "C" EMSCRIPTEN_KEEPALIVE void ftk_clipboardPaste(const char* text)
{
    // The system is set directly rather than through an SDL clipboard
    // event: the paste shortcut's key event is already in SDL's queue,
    // and an event pushed now would be processed after it.
    if (ftk::clipboardSystemInstance)
    {
        ftk::clipboardSystemInstance->setText(text);
    }
}

EM_JS_DEPS(ftk_clipboardDeps, "$stringToNewUTF8");

// SDL's web port has no clipboard, so the browser's paste event feeds
// this one. The event fires between the key event and the frame that
// processes it, so the text is there by the time the paste shortcut is
// handled.
EM_JS(void, ftk_clipboardInit, (), {
    document.addEventListener('paste', function(e)
    {
        var text = e.clipboardData ? e.clipboardData.getData('text/plain') : '';
        if (text)
        {
            var p = stringToNewUTF8(text);
            _ftk_clipboardPaste(p);
            _free(p);
        }
        e.preventDefault();
    });
});
#endif // __EMSCRIPTEN__

namespace ftk
{
    struct ClipboardSystem::Private
    {
        std::shared_ptr<ftk::Observable<std::string> > text;
    };

    ClipboardSystem::ClipboardSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::ClipboardSystem"),
        _p(new Private)
    {
        FTK_P();
        p.text = Observable<std::string>::create();
#if defined(__EMSCRIPTEN__)
        clipboardSystemInstance = this;
        ftk_clipboardInit();
#endif // __EMSCRIPTEN__
    }

    ClipboardSystem::~ClipboardSystem()
    {
#if defined(__EMSCRIPTEN__)
        clipboardSystemInstance = nullptr;
#endif // __EMSCRIPTEN__
    }

    std::shared_ptr<ClipboardSystem> ClipboardSystem::create(const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<ClipboardSystem>(new ClipboardSystem(context));
    }

    const std::string& ClipboardSystem::getText() const
    {
        return _p->text->get();
    }

    std::shared_ptr<ftk::IObservable<std::string> > ClipboardSystem::observeText() const
    {
        return _p->text;
    }

    void ClipboardSystem::setText(const std::string& value)
    {
        if (_p->text->setIfChanged(value))
        {
            SDL_SetClipboardText(value.c_str());
#if defined(__EMSCRIPTEN__)
            // Mirror to the page's clipboard, so a copy here can be
            // pasted outside. The write is only allowed near a user
            // gesture, which a copy always is.
            EM_ASM({
                var text = UTF8ToString($0);
                if (navigator.clipboard && navigator.clipboard.writeText)
                {
                    navigator.clipboard.writeText(text).catch(function() {});
                }
            }, value.c_str());
#endif // __EMSCRIPTEN__
        }
    }
}

