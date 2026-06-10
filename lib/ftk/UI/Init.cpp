// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Init.h>

#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/IconSystem.h>

#if defined(FTK_API_GL_4_1) || defined(FTK_API_GLES_2)
#include <ftk/GL/Init.h>
#endif // FTK_API_GL_4_1

#include <ftk/Core/Context.h>
#include <ftk/Core/DiagSystem.h>

namespace ftk
{
    void uiInit(const std::shared_ptr<Context>& context)
    {
#if defined(FTK_API_GL_4_1) || defined(FTK_API_GLES_2)
        gl::init(context);
#endif // FTK_API_GL_4_1

        auto diagSystem = context->getSystem<DiagSystem>();
        diagSystem->addSampler(
            "ftk Objects/IWidgets: {0}",
            [] { return IWidget::getObjectCount(); });

        if (!context->getSystem<ClipboardSystem>())
        {
            context->addSystem(ClipboardSystem::create(context));
        }
        if (!context->getSystem<DialogSystem>())
        {
            context->addSystem(DialogSystem::create(context));
        }
        if (!context->getSystem<FileBrowserSystem>())
        {
            context->addSystem(FileBrowserSystem::create(context));
        }
        if (!context->getSystem<IconSystem>())
        {
            context->addSystem(IconSystem::create(context));
        }
    }
}
