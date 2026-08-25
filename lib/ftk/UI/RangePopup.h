// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/DoubleModel.h>
#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IntModel.h>
#include <ftk/UI/IWidgetPopup.h>

namespace ftk
{
    //! \name Numeric Widgets
    ///@{

    //! Popup for editing the range of a floating point model.
    class FTK_UI_API_TYPE FloatRangePopup : public IWidgetPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatRangePopup();

    public:
        FTK_UI_API virtual ~FloatRangePopup();

        //! Create a new popup.
        FTK_UI_API static std::shared_ptr<FloatRangePopup> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

    private:
        FTK_PRIVATE();
    };

    //! Popup for editing the range of a double precision floating point
    //! model.
    class FTK_UI_API_TYPE DoubleRangePopup : public IWidgetPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleRangePopup();

    public:
        FTK_UI_API virtual ~DoubleRangePopup();

        //! Create a new popup.
        FTK_UI_API static std::shared_ptr<DoubleRangePopup> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

    private:
        FTK_PRIVATE();
    };

    //! Popup for editing the range of an integer model.
    class FTK_UI_API_TYPE IntRangePopup : public IWidgetPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntRangePopup();

    public:
        FTK_UI_API virtual ~IntRangePopup();

        //! Create a new popup.
        FTK_UI_API static std::shared_ptr<IntRangePopup> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
