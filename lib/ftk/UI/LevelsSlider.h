// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color levels model.
    class FTK_API_TYPE LevelsModel : public std::enable_shared_from_this<LevelsModel>
    {
        FTK_NON_COPYABLE(LevelsModel);

    protected:
        LevelsModel();

    public:
        ~LevelsModel();

        //! Create a new model.
        static std::shared_ptr<LevelsModel> create();

        //! \name Value
        ///@{

        const RangeF& getValue() const;
        std::shared_ptr<IObservable<RangeF> > observeValue() const;
        void setValue(const RangeF&);

        ///@}

        //! \name Range
        ///@{

        const RangeF& getRange() const;
        std::shared_ptr<IObservable<RangeF> > observeRange() const;
        void setRange(const RangeF&);

        ///@}

        //! \name Increment
        ///@{

        float getStep() const;
        void setStep(float);

        float getLargeStep() const;
        void setLargeStep(float);

        ///@}

        //! \name Default Value
        ///@{

        //! Get whether there is a default value.
        bool hasDefault() const;

        //! Observe whether there is a default value.
        std::shared_ptr<IObservable<bool> > observeHasDefault() const;

        //! Get the default value.
        const RangeF& getDefault() const;

        //! Observe the default value.
        std::shared_ptr<IObservable<RangeF> > observeDefault() const;

        //! Set the default value.
        void setDefault(const RangeF&);

        //! Set the value to the default value.
        void setDefault();

        //! Clear the default value.
        void clearDefault();

        ///@}

    private:
        FTK_PRIVATE();
    };

    //! Color levels slider.
    class FTK_API_TYPE LevelsSlider : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LevelsModel>&,
            const std::shared_ptr<IWidget>& parent);

        LevelsSlider();

    public:
        FTK_API virtual ~LevelsSlider();

        //! Create a new widget.
        FTK_API static std::shared_ptr<LevelsSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<LevelsSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LevelsModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_API const RangeF& getValue() const;
        FTK_API void setValue(const RangeF&);
        FTK_API void setCallback(const std::function<void(const RangeF&)>&);

        //! Set the value callback with a flag for whether the slider is presssed.
        FTK_API void setPressedCallback(const std::function<void(const RangeF&, bool)>&);

        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeF& getRange() const;
        FTK_API void setRange(const RangeF&);
        FTK_API void setRange(float, float);

        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<LevelsModel>& getModel() const;

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void styleEvent(const StyleEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        Box2I _getInsideGeometry() const;
        Box2I _getSliderGeometry() const;
        Box2I _getMinHandleGeometry() const;
        Box2I _getMaxHandleGeometry() const;

        float _posToValue(int) const;
        int _valueToPos(float) const;

        FTK_PRIVATE();
    };

    //! Color levels editors and slider.
    class FTK_API_TYPE LevelsEditSlider : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        LevelsEditSlider();

    public:
        FTK_API virtual ~LevelsEditSlider();

        //! Create a new widget.
        FTK_API static std::shared_ptr<LevelsEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        FTK_API const RangeF& getValue() const;
        FTK_API void setValue(const RangeF&);
        FTK_API void setCallback(const std::function<void(const RangeF&)>&);

        //! Set the value callback with a flag for whether the slider is presssed.
        FTK_API void setPressedCallback(const std::function<void(const RangeF&, bool)>&);

        ///@}

        //! \name Range
        ///@{

        FTK_API const RangeF& getRange() const;
        FTK_API void setRange(const RangeF&);
        FTK_API void setRange(float, float);

        ///@}

        //! Get the model.
        FTK_API const std::shared_ptr<LevelsModel>& getModel() const;

        //! Get the display precision.
        FTK_API int getPrecision() const;

        //! Set the display precision.
        FTK_API void setPrecision(int);

        //! Get the font.
        FTK_API FontType getFont() const;

        //! Set the font.
        FTK_API void setFont(FontType);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
