// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/FloatSlider.h>
#include <ftk/UI/IntSlider.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color integer slider.
    class FTK_UI_API_TYPE ColorIntSlider : public IIntSlider
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorIntSlider();

    public:
        FTK_UI_API virtual ~ColorIntSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorIntSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the colors.
        FTK_UI_API void setColors(const std::vector<V4F>&);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_UI_API Box2I _getSliderGeometry() const override;

    private:
        Box2I _getInsideGeometry() const;

        FTK_PRIVATE();
    };

    //! Color integer editor and slider.
    class FTK_UI_API_TYPE ColorIntEditSlider : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorIntEditSlider();

    public:
        FTK_UI_API virtual ~ColorIntEditSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorIntEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorIntEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IntModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_UI_API const std::shared_ptr<IntModel>& getModel() const;

        //! Set the colors.
        FTK_UI_API void setColors(const std::vector<V4F>&);

        //! \name Value
        ///@{

        FTK_UI_API int getValue() const;
        FTK_UI_API void setValue(int);
        FTK_UI_API void setCallback(const std::function<void(int)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(int, bool)>&);

        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeI& getRange() const;
        FTK_UI_API void setRange(const RangeI&);
        FTK_UI_API void setRange(int, int);

        ///@}


    private:
        FTK_PRIVATE();
    };

    //! Color floating point slider.
    class FTK_UI_API_TYPE ColorFloatSlider : public IFloatSlider
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorFloatSlider();

    public:
        FTK_UI_API virtual ~ColorFloatSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorFloatSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the colors.
        FTK_UI_API void setColors(const std::vector<V4F>&);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_UI_API Box2I _getSliderGeometry() const override;

    private:
        Box2I _getInsideGeometry() const;

        FTK_PRIVATE();
    };

    //! Color floating point editor and slider.
    class FTK_UI_API_TYPE ColorFloatEditSlider : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent);

        ColorFloatEditSlider();

    public:
        FTK_UI_API virtual ~ColorFloatEditSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorFloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorFloatEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FloatModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_UI_API const std::shared_ptr<FloatModel>& getModel() const;

        //! Set the colors.
        FTK_UI_API void setColors(const std::vector<V4F>&);

        //! \name Value
        ///@{

        FTK_UI_API float getValue() const;
        FTK_UI_API void setValue(float);
        FTK_UI_API void setCallback(const std::function<void(float)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(float, bool)>&);

        ///@}

        //! \name Range
        ///@{

        FTK_UI_API const RangeF& getRange() const;
        FTK_UI_API void setRange(const RangeF&);
        FTK_UI_API void setRange(float, float);

        ///@}


    private:
        FTK_PRIVATE();
    };

    ///@}
}
