// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Dialogs.h"

#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/FileEdit.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Dialogs::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Dialogs", nullptr);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Message dialog.
        auto button = PushButton::create(context, "Message Dialog", layout);
        button->setClickedCallback(
            [this]
            {
                if (auto context = getContext())
                {
                    context->getSystem<DialogSystem>()->message(
                        "Message",
                        "Hello world!",
                        getWindow());
                }
            });

        // Confirmation dialog.
        button = PushButton::create(context, "Confirmation Dialog", layout);
        button->setClickedCallback(
            [this]
            {
                if (auto context = getContext())
                {
                    context->getSystem<DialogSystem>()->confirm(
                        "Confirm",
                        "Hello world?",
                        getWindow(),
                        [](bool value)
                        {
                            std::cout << "Hello world: " << value << std::endl;
                        });
                }
            });

        // Progress dialog.
        _progressTimer = Timer::create(context);
        _progressTimer->setRepeating(true);
        button = PushButton::create(context, "Progress Dialog", layout);
        button->setClickedCallback(
            [this]
            {
                if (auto context = getContext())
                {
                    const double inc = 0.005;
                    const double max = 1.0;
                    _progressDialog = ProgressDialog::create(
                        context,
                        "Progress",
                        "In progress:");
                    _progressDialog->setMessage(Format("{0} / {1}").arg(0).arg(int(max / inc)));
                    _progressDialog->setCloseCallback(
                        [this]
                        {
                            _progressTimer->stop();
                            _progressDialog.reset();
                        });
                    _progressDialog->open(getWindow());
                    _progressTimer->start(
                        std::chrono::microseconds(500),
                        [this, max, inc]
                        {
                            double v = _progressDialog->getValue();
                            v += 0.005;
                            if (v < 1.0)
                            {
                                _progressDialog->setValue(v);
                                _progressDialog->setMessage(
                                    Format("{0} / {1}").arg(v * (max / inc)).arg(int(max / inc)));
                            }
                            else
                            {
                                _progressDialog->close();
                            }
                        });
                }
            });

        // File browsers.
        auto fileEdit = FileEdit::create(context, layout);
        fileEdit->setPath(Path("File Browser"));
        auto dirEdit = FileEdit::create(context, FileBrowserMode::Dir, layout);
        dirEdit->setPath(Path("Directory Browser"));
    }

    Dialogs::~Dialogs()
    {}

    std::shared_ptr<Dialogs> Dialogs::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Dialogs>(new Dialogs);
        out->_init(context);
        return out;
    }
    
    Size2I Dialogs::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Dialogs::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
