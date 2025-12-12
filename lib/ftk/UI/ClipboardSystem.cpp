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
    {};

    ClipboardSystem::ClipboardSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::ClipboardSystem"),
        _p(new Private)
    {}

    ClipboardSystem::~ClipboardSystem()
    {}

    std::shared_ptr<ClipboardSystem> ClipboardSystem::create(const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<ClipboardSystem>(new ClipboardSystem(context));
    }

    std::string ClipboardSystem::getText() const
    {
        std::string out;
        char* buf = SDL_GetClipboardText();
        if (buf)
        {
            out = std::string(buf);
        }
        //! \bug This is causing segfaults in the tests on Linux:
        //SDL_free(&buf);
        return out;
    }

    void ClipboardSystem::setText(const std::string& value)
    {
        SDL_SetClipboardText(value.c_str());
    }
}
