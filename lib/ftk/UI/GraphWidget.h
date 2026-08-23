// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IContainer.h>
#include <ftk/UI/IWidget.h>

#include <ftk/Core/DiagSystem.h>

namespace ftk
{
    //! \name Graph Widgets
    ///@{

    //! One line on a graph: its colour, the format string for its readout,
    //! and how the sample is turned into the number that format expects.
    struct FTK_API_TYPE GraphLabel
    {
        ColorRole colorRole = ColorRole::None;
        std::string text;
        DiagFormat format;
    };

    //! Graph widget.
    class FTK_API_TYPE GraphWidget : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::vector<GraphLabel>& labels,
            const std::shared_ptr<IWidget>& parent);

        GraphWidget();

    public:
        FTK_API virtual ~GraphWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<GraphWidget> create(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::vector<GraphLabel>& labels,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the samples.
        FTK_API void setSamples(ColorRole, const std::vector<int64_t>&);

        //! Add a sample.
        FTK_API void addSample(ColorRole, int64_t);


    private:
        FTK_PRIVATE();
    };

    ///@}
}
