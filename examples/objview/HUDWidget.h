// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DocumentModel.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Label.h>

namespace objview
{
    //! Heads up display (HUD) widget.
    class HUDWidget : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<ftk::IDocument>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        HUDWidget() = default;

    public:
        virtual ~HUDWidget();

        //! Create a new widget.
        static std::shared_ptr<HUDWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<ftk::IDocument>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

        void setOrbit(const ftk::V2F&);
        void setDistance(float);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::map<std::string, std::shared_ptr<ftk::Label> > _labels;
        std::shared_ptr<ftk::GridLayout> _layout;
    };
}
