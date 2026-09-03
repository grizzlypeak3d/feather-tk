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

    //! Popup with a slider's options: reset, and the range when
    //! it is soft.
    class FTK_UI_API_TYPE FloatSliderPopup : public IWidgetPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        FloatSliderPopup();

    public:
        FTK_UI_API virtual ~FloatSliderPopup();

        //! Create a new popup.
        FTK_UI_API static std::shared_ptr<FloatSliderPopup> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

    private:
        FTK_PRIVATE();
    };

    //! Popup with a slider's options, for a double precision floating
    //! point model.
    class FTK_UI_API_TYPE DoubleSliderPopup : public IWidgetPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent);

        DoubleSliderPopup();

    public:
        FTK_UI_API virtual ~DoubleSliderPopup();

        //! Create a new popup.
        FTK_UI_API static std::shared_ptr<DoubleSliderPopup> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DoubleModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

    private:
        FTK_PRIVATE();
    };

    //! Popup with a slider's options, for an integer model.
    class FTK_UI_API_TYPE IntSliderPopup : public IWidgetPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        IntSliderPopup();

    public:
        FTK_UI_API virtual ~IntSliderPopup();

        //! Create a new popup.
        FTK_UI_API static std::shared_ptr<IntSliderPopup> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
