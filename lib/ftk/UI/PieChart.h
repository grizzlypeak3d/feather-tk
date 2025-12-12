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
        FTK_API PieChartData(
            float                percentage,
            const Color4F& color);

        float         percentage = 0.F;
        Color4F color;

        FTK_API bool operator == (const PieChartData&) const;
        FTK_API bool operator != (const PieChartData&) const;
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
        FTK_API virtual ~PieChart();

        //! Create a new widget.
        FTK_API static std::shared_ptr<PieChart> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the pie chart data.
        FTK_API const std::vector<PieChartData>& getData() const;

        //! Set the pie chart data.
        FTK_API void setData(const std::vector<PieChartData>&);

        //! Set the size multiplier.
        FTK_API int getSizeMult() const;

        //! Set the size multiplier.
        FTK_API void setSizeMult(int);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
