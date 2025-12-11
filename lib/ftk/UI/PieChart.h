// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Chart Widgets
    ///@{

    //! Pie chart data.
    struct FTK_API_TYPE PieChartData
    {
        PieChartData() = default;
        PieChartData(
            float                percentage,
            const Color4F& color);

        float         percentage = 0.F;
        Color4F color;

        bool operator == (const PieChartData&) const;
        bool operator != (const PieChartData&) const;
    };

    //! Pie chart widget.
    class FTK_API_TYPE PieChart : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        PieChart();

    public:
        virtual ~PieChart();

        //! Create a new widget.
        static std::shared_ptr<PieChart> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the pie chart data.
        const std::vector<PieChartData>& getData() const;

        //! Set the pie chart data.
        void setData(const std::vector<PieChartData>&);

        //! Set the size multiplier.
        int getSizeMult() const;

        //! Set the size multiplier.
        void setSizeMult(int);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
