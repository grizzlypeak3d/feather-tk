// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/App.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/IDialog.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>

using namespace ftk;

class AboutDialog : public IDialog
{
    FTK_NON_COPYABLE(AboutDialog);

protected:
    void _init(
        const std::shared_ptr<Context>&,
        const std::shared_ptr<IWidget>& parent);

    AboutDialog();

public:
    virtual ~AboutDialog();

    static std::shared_ptr<AboutDialog> create(
        const std::shared_ptr<Context>&,
        const std::shared_ptr<IWidget>& parent = nullptr);
private:
};

void AboutDialog::_init(
    const std::shared_ptr<Context>& context,
    const std::shared_ptr<IWidget>& parent)
{
    IDialog::_init(
        context,
        "djv::app::AboutDialog",
        parent);

    auto titleLabel = Label::create(context, "About");
    titleLabel->setMarginRole(SizeRole::MarginSmall);

    auto licensesButton = PushButton::create(context, "Licenses");
    auto closeButton = PushButton::create(context, "Close");

    auto layout = VerticalLayout::create(context, shared_from_this());
    layout->setSpacingRole(SizeRole::None);
    titleLabel->setParent(layout);
    Divider::create(context, Orientation::Vertical, layout);

    auto vLayout = VerticalLayout::create(context);
    vLayout->setMarginRole(SizeRole::MarginSmall);
    vLayout->setSpacingRole(SizeRole::Spacing);
    Label::create(
        context,
        "DJV_VERSION_FULL",
        vLayout);
    Label::create(
        context,
        "Copyright Contributors to the DJV project.",
        vLayout);
    licensesButton->setParent(vLayout);
    auto scrollWidget = ScrollWidget::create(context, ScrollType::Both, layout);
    scrollWidget->setBorder(false);
    //scrollWidget->setScrollBarsAutoHide(false);
    scrollWidget->setWidget(vLayout);

    Divider::create(context, Orientation::Vertical, layout);
    auto hLayout = HorizontalLayout::create(context, layout);
    hLayout->setMarginRole(SizeRole::MarginSmall);
    hLayout->addSpacer(SizeRole::Spacing, Stretch::Expanding);
    closeButton->setParent(hLayout);

    closeButton->setClickedCallback(
        [this]
        {
            close();
        });

    licensesButton->setClickedCallback(
        [this]
        {
        });
}

AboutDialog::AboutDialog()
{}

AboutDialog::~AboutDialog()
{}

std::shared_ptr<AboutDialog> AboutDialog::create(
    const std::shared_ptr<Context>& context,
    const std::shared_ptr<IWidget>& parent)
{
    auto out = std::shared_ptr<AboutDialog>(new AboutDialog);
    out->_init(context, parent);
    return out;
}

int main(int argc, char** argv)
{
    // Create the context and application.
    auto context = Context::create();
    auto app = App::create(context, argc, argv, "simple", "Simple example");
    if (app->getExit() != 0)
        return app->getExit();

    // Create a window.
    auto window = MainWindow::create(context, app, Size2I(1280, 960));

    auto aboutDialog = AboutDialog::create(context);
    aboutDialog->open(window);

    // Run the application.
    app->run();
    return 0;
}
