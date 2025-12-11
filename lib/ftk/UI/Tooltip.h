// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IPopup.h>

namespace ftk
{
    //! \name Popup Widgets
    ///@{

    constexpr std::chrono::milliseconds tooltipTimeout(1000);

    //! Tooltip.
    class FTK_API_TYPE Tooltip : public IPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const V2I& pos,
            const std::shared_ptr<IWidget>&);

        Tooltip();

    public:
        virtual ~Tooltip();

        //! Create a new tooltip.
        static std::shared_ptr<Tooltip> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const V2I& pos,
            const std::shared_ptr<IWidget>&);

        void close() override;

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
