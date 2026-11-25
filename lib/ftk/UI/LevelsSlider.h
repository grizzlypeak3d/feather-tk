// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color levels model.
    class FTK_UI_API_TYPE LevelsModel : public std::enable_shared_from_this<LevelsModel>
    {
        FTK_NON_COPYABLE(LevelsModel);

    protected:
        LevelsModel();

    public:
        FTK_UI_API ~LevelsModel();

        //! Create a new model.
        FTK_UI_API static std::shared_ptr<LevelsModel> create();

        //! \name Value
        ///@{

        //! Get the value, the minimum and maximum levels, which are kept within
        //! the range. The default is 0 to 1.
        FTK_UI_API const RangeF& getValue() const;
        //! Observe the value.
        FTK_UI_API std::shared_ptr<IObservable<RangeF> > observeValue() const;
        //! Set the value. Its minimum and maximum are each clamped to the
        //! range.
        FTK_UI_API void setValue(const RangeF&);

        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within. The default is 0 to 1.
        FTK_UI_API const RangeF& getRange() const;
        //! Observe the range.
        FTK_UI_API std::shared_ptr<IObservable<RangeF> > observeRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeF&);

        ///@}

        //! \name Increment
        ///@{

        //! Get the step. The default is 0.1.
        FTK_UI_API float getStep() const;
        //! Set the step.
        FTK_UI_API void setStep(float);

        //! Get the large step. The default is 1.
        FTK_UI_API float getLargeStep() const;
        //! Set the large step.
        FTK_UI_API void setLargeStep(float);

        ///@}

        //! \name Default Value
        ///@{

        //! Get whether there is a default value.
        FTK_UI_API bool hasDefault() const;

        //! Observe whether there is a default value.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeHasDefault() const;

        //! Get the default value.
        FTK_UI_API const RangeF& getDefault() const;

        //! Observe the default value.
        FTK_UI_API std::shared_ptr<IObservable<RangeF> > observeDefault() const;

        //! Set the default value.
        FTK_UI_API void setDefault(const RangeF&);

        //! Set the value to the default value.
        FTK_UI_API void setDefault();

        //! Clear the default value.
        FTK_UI_API void clearDefault();

        ///@}

    private:
        FTK_PRIVATE();
    };

    //! Color levels slider.
    class FTK_UI_API_TYPE LevelsSlider : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LevelsModel>&,
            const std::shared_ptr<IWidget>& parent);

        LevelsSlider();

    public:
        FTK_UI_API virtual ~LevelsSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<LevelsSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<LevelsSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LevelsModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        //! Get the value, the minimum and maximum levels, which are kept within
        //! the range. Dragging one handle past the other moves the other with
        //! it.
        FTK_UI_API const RangeF& getValue() const;
        //! Set the value. Its minimum and maximum are each clamped to the
        //! range.
        FTK_UI_API void setValue(const RangeF&);
        //! Set the callback, called with the value whenever it changes,
        //! including on every move while a handle is dragged, from setValue(),
        //! and from other widgets that share the model.
        FTK_UI_API void setCallback(const std::function<void(const RangeF&)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(const RangeF&, bool)>&);

        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within.
        FTK_UI_API const RangeF& getRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeF&);
        //! Set the range from its minimum and maximum.
        FTK_UI_API void setRange(float, float);

        ///@}

        //! Get the model.
        FTK_UI_API const std::shared_ptr<LevelsModel>& getModel() const;

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_UI_API void mouseLeaveEvent() override;
        FTK_UI_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void mouseReleaseEvent(MouseClickEvent&) override;

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
    class FTK_UI_API_TYPE LevelsEditSlider : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        LevelsEditSlider();

    public:
        FTK_UI_API virtual ~LevelsEditSlider();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<LevelsEditSlider> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Value
        ///@{

        //! Get the value, the minimum and maximum levels, which are kept within
        //! the range. Dragging or typing one past the other moves the other
        //! with it.
        FTK_UI_API const RangeF& getValue() const;
        //! Set the value. Its minimum and maximum are each clamped to the
        //! range.
        FTK_UI_API void setValue(const RangeF&);
        //! Set the callback, called with the value whenever it changes,
        //! including from the edits, on every move while a handle is dragged,
        //! and from setValue().
        FTK_UI_API void setCallback(const std::function<void(const RangeF&)>&);

        //! Set the value callback with a flag for whether the slider is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(const RangeF&, bool)>&);

        ///@}

        //! \name Range
        ///@{

        //! Get the range the value is kept within.
        FTK_UI_API const RangeF& getRange() const;
        //! Set the range. The value is clamped to the new range.
        FTK_UI_API void setRange(const RangeF&);
        //! Set the range from its minimum and maximum.
        FTK_UI_API void setRange(float, float);

        ///@}

        //! Get the model.
        FTK_UI_API const std::shared_ptr<LevelsModel>& getModel() const;

        //! Get the display precision.
        FTK_UI_API int getPrecision() const;

        //! Set the display precision.
        FTK_UI_API void setPrecision(int);

        //! Get the font.
        FTK_UI_API FontType getFont() const;

        //! Set the font.
        FTK_UI_API void setFont(FontType);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
