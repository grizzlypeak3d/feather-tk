// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Graph Widgets
    ///@{

    //! Graph widget.
    class FTK_API_TYPE GraphWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        GraphWidget();

    public:
        FTK_API virtual ~GraphWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<GraphWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void addSample(int, ColorRole = ColorRole::Cyan);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        void _samplesUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
