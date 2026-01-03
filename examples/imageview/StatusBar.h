// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DocumentModel.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>

namespace imageview
{
    class App;

    //! Status bar widget.
    class StatusBar : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        StatusBar() = default;

    public:
        virtual ~StatusBar();

        //! Create a new widget.
        static std::shared_ptr<StatusBar> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::map<std::string, std::shared_ptr<ftk::Label> > _labels;
        std::shared_ptr<ftk::HorizontalLayout> _layout;

        std::shared_ptr<ftk::Observer<std::shared_ptr<ftk::IDocument> > > _currentObserver;
    };
}
