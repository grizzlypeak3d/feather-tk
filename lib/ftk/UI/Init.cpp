// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Init.h>

#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/IconSystem.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/DiagSystem.h>

namespace ftk
{
    void uiInit(const std::shared_ptr<Context>& context)
    {
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
