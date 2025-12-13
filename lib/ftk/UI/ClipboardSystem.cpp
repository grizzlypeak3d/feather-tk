// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ClipboardSystem.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

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
    }

    ClipboardSystem::~ClipboardSystem()
    {}

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
        }
    }
}

