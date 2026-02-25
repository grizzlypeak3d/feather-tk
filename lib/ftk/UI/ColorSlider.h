// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FloatSlider.h>
#include <ftk/UI/IntSlider.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color integer slider.
    class FTK_API_TYPE ColorIntSlider : public IIntSlider
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorIntSlider();

    public:
        FTK_API virtual ~ColorIntSlider();

        FTK_API static std::shared_ptr<ColorIntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the colors.
        void setColors(const std::vector<V4F>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_API Box2I _getSliderGeometry() const override;

    private:
        FTK_PRIVATE();
    };

    //! Color floating point slider.
    class FTK_API_TYPE ColorFloatSlider : public IFloatSlider
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorFloatSlider();

    public:
        FTK_API virtual ~ColorFloatSlider();

        FTK_API static std::shared_ptr<ColorFloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the colors.
        void setColors(const std::vector<V4F>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_API Box2I _getSliderGeometry() const override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
