// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ComboBox.h>
#include <ftk/UI/ScrollWidget.h>

namespace widgets
{
    class App;

    class Fonts : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        Fonts() = default;

    public:
        virtual ~Fonts();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::vector<std::string> _fonts;

        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;
        std::map<ftk::FontRole, std::shared_ptr<ftk::ComboBox> > _comboBoxes;

        std::shared_ptr<ftk::ListObserver<std::string> > _fontsObserver;
        std::shared_ptr<ftk::MapObserver<ftk::FontRole, ftk::FontInfo> > _fontRolesObserver;
    };
}
