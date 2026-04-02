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

        //! Create a new widget.
        FTK_API static std::shared_ptr<ColorIntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the colors.
        FTK_API void setColors(const std::vector<V4F>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void styleEvent(const StyleEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_API Box2I _getSliderGeometry() const override;

    private:
        Box2I _getInsideGeometry() const;

        FTK_PRIVATE();
    };

    //! Color integer editor and slider.
    class FTK_API_TYPE ColorIntEditSlider : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorIntEditSlider();

    public:
        FTK_API virtual ~ColorIntEditSlider();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ColorIntEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<ColorIntEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_API const std::shared_ptr<IntModel>& getModel() const;

        //! Set the colors.
        FTK_API void setColors(const std::vector<V4F>&);

        //! \name Value
        ///@{

        FTK_API int getValue() const;
        FTK_API void setValue(int);
        FTK_API void setCallback(const std::function<void(int)>&);

        //! Set the value callback with a flag for whether the slider is presssed.
        FTK_API void setPressedCallback(const std::function<void(int, bool)>&);

        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeI& getRange() const;
        FTK_API void setRange(const RangeI&);
        FTK_API void setRange(int, int);

        ///@}

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

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

        //! Create a new widget.
        FTK_API static std::shared_ptr<ColorFloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the colors.
        FTK_API void setColors(const std::vector<V4F>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void styleEvent(const StyleEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_API Box2I _getSliderGeometry() const override;

    private:
        Box2I _getInsideGeometry() const;

        FTK_PRIVATE();
    };

    //! Color floating point editor and slider.
    class FTK_API_TYPE ColorFloatEditSlider : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorFloatEditSlider();

    public:
        FTK_API virtual ~ColorFloatEditSlider();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ColorFloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<ColorFloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_API const std::shared_ptr<FloatModel>& getModel() const;

        //! Set the colors.
        FTK_API void setColors(const std::vector<V4F>&);

        //! \name Value
        ///@{

        FTK_API float getValue() const;
        FTK_API void setValue(float);
        FTK_API void setCallback(const std::function<void(float)>&);

        //! Set the value callback with a flag for whether the slider is presssed.
        FTK_API void setPressedCallback(const std::function<void(float, bool)>&);

        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeF& getRange() const;
        FTK_API void setRange(const RangeF&);
        FTK_API void setRange(float, float);

        ///@}

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
