// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/LevelsSlider.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

namespace ftk
{
    struct LevelsModel::Private
    {
        std::shared_ptr<Observable<RangeF> > value;
        std::shared_ptr<Observable<RangeF> > range;
        float step = .1F;
        float largeStep = 1.F;
        std::shared_ptr<Observable<bool> > hasDefault;
        std::shared_ptr<Observable<RangeF> > defaultValue;
    };

    LevelsModel::LevelsModel() :
        _p(new Private)
    {
        FTK_P();
        p.value = Observable<RangeF>::create(RangeF(0.F, 1.F));
        p.range = Observable<RangeF>::create(RangeF(0.F, 1.F));
        p.hasDefault = Observable<bool>::create(false);
        p.defaultValue = Observable<RangeF>::create(RangeF(0.F, 1.F));
    }

    LevelsModel::~LevelsModel()
    {}

    std::shared_ptr<LevelsModel> LevelsModel::create()
    {
        return std::shared_ptr<LevelsModel>(new LevelsModel);
    }

    const RangeF& LevelsModel::getValue() const
    {
        return _p->value->get();
    }

    std::shared_ptr<IObservable<RangeF> > LevelsModel::observeValue() const
    {
        return _p->value;
    }

    void LevelsModel::setValue(const RangeF& value)
    {
        FTK_P();
        const RangeF& range = p.range->get();
        const RangeF tmp(
            clamp(value.min(), range.min(), range.max()),
            clamp(value.max(), range.min(), range.max()));
        _p->value->setIfChanged(tmp);
    }

    const RangeF& LevelsModel::getRange() const
    {
        return _p->range->get();
    }

    std::shared_ptr<IObservable<RangeF> > LevelsModel::observeRange() const
    {
        return _p->range;
    }

    void LevelsModel::setRange(const RangeF& range)
    {
        FTK_P();
        if (p.range->setIfChanged(range))
        {
            setValue(p.value->get());
        }
    }

    float LevelsModel::getStep() const
    {
        return _p->step;
    }

    void LevelsModel::setStep(float value)
    {
        _p->step = value;
    }

    float LevelsModel::getLargeStep() const
    {
        return _p->largeStep;
    }

    void LevelsModel::setLargeStep(float value)
    {
        _p->largeStep = value;
    }

    bool LevelsModel::hasDefault() const
    {
        return _p->hasDefault->get();
    }

    std::shared_ptr<IObservable<bool> > LevelsModel::observeHasDefault() const
    {
        return _p->hasDefault;
    }

    const RangeF& LevelsModel::getDefault() const
    {
        return _p->defaultValue->get();
    }

    std::shared_ptr<IObservable<RangeF> > LevelsModel::observeDefault() const
    {
        return _p->defaultValue;
    }

    void LevelsModel::setDefault(const RangeF& value)
    {
        _p->defaultValue->setIfChanged(value);
        _p->hasDefault->setIfChanged(true);
    }

    void LevelsModel::setDefault()
    {
        setValue(_p->defaultValue->get());
    }

    void LevelsModel::clearDefault()
    {
        _p->hasDefault->setIfChanged(false);
    }

    struct LevelsSlider::Private
    {
        std::shared_ptr<LevelsModel> model;
        std::function<void(const RangeF&)> callback;
        std::function<void(const RangeF&, bool)> pressedCallback;
        int blockCallbacks = 0;
        std::shared_ptr<Observer<RangeF> > valueObserver;
        std::shared_ptr<Observer<RangeF> > rangeObserver;

        struct SizeData
        {
            std::optional<float> displayScale;
            int size = 0;
            int border = 0;
            int handle = 0;
            Size2I sizeHint;
            Box2I g;
            Box2I g2;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F border;
        };
        std::optional<DrawData> draw;

        struct MouseData
        {
            int handle = -1;
        };
        MouseData mouse;
    };

    void LevelsSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<LevelsModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::LevelsSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.model = model;

        p.valueObserver = Observer<RangeF>::create(
            p.model->observeValue(),
            [this](const RangeF& value)
            {
                FTK_P();
                setSizeUpdate();
                setDrawUpdate();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, _isMousePressed());
                }
            });

        p.rangeObserver = Observer<RangeF>::create(
            p.model->observeRange(),
            [this](const RangeF&)
            {
                setSizeUpdate();
                setDrawUpdate();
            });
    }

    LevelsSlider::LevelsSlider() :
        _p(new Private)
    {}

    LevelsSlider::~LevelsSlider()
    {}

    std::shared_ptr<LevelsSlider> LevelsSlider::create(
        const std::shared_ptr<Context>&context,
        const std::shared_ptr<IWidget>&parent)
    {
        auto out = std::shared_ptr<LevelsSlider>(new LevelsSlider);
        out->_init(context, LevelsModel::create(), parent);
        return out;
    }

    std::shared_ptr<LevelsSlider> LevelsSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<LevelsModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<LevelsSlider>(new LevelsSlider);
        out->_init(context, model, parent);
        return out;
    }

    const RangeF& LevelsSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void LevelsSlider::setValue(const RangeF& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void LevelsSlider::setCallback(const std::function<void(const RangeF&)>& value)
    {
        _p->callback = value;
    }

    void LevelsSlider::setPressedCallback(const std::function<void(const RangeF&, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeF& LevelsSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void LevelsSlider::setRange(const RangeF& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void LevelsSlider::setRange(float min, float max)
    {
        setRange(RangeF(min, max));
    }

    const std::shared_ptr<LevelsModel>& LevelsSlider::getModel() const
    {
        return _p->model;
    }

    Size2I LevelsSlider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void LevelsSlider::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.size.g = margin(value, -p.size.handle / 2, 0, -p.size.handle / 2, -p.size.handle);
            p.size.g2 = margin(p.size.g, -p.size.border);
            p.draw.reset();
        }
    }

    void LevelsSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFontRole(FontRole::Label, event.displayScale);
            p.size.sizeHint = Size2I(p.size.size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.border);

            p.draw.reset();
        }
    }

    void LevelsSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IMouseWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void LevelsSlider::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->border = border(margin(p.size.g2, p.size.border), p.size.border);
        }

        // Draw the border.
        event.render->drawMesh(
            p.draw->border,
            event.style->getColorRole(ColorRole::Border));

        // Draw the colors.
        TriMesh2F mesh;
        int w = p.size.g2.w();
        int h = p.size.g2.h();
        const int v0 = _valueToPos(getValue().min());
        const int v1 = _valueToPos(getValue().max());
        mesh.v.push_back(V2F(p.size.g2.min.x, p.size.g2.min.y));
        mesh.v.push_back(V2F(p.size.g2.min.x, p.size.g2.max.y + 1));
        mesh.c.push_back(V4F(0.F, 0.F, 0.F, 1.F));
        mesh.v.push_back(V2F(v0, p.size.g2.min.y));
        mesh.v.push_back(V2F(v0, p.size.g2.max.y + 1));
        mesh.c.push_back(V4F(0.F, 0.F, 0.F, 1.F));
        mesh.v.push_back(V2F(v1, p.size.g2.min.y));
        mesh.v.push_back(V2F(v1, p.size.g2.max.y + 1));
        mesh.c.push_back(V4F(1.F, 1.F, 1.F, 1.F));
        mesh.v.push_back(V2F(p.size.g2.max.x + 1, p.size.g2.min.y));
        mesh.v.push_back(V2F(p.size.g2.max.x + 1, p.size.g2.max.y + 1));
        mesh.c.push_back(V4F(1.F, 1.F, 1.F, 1.F));
        mesh.triangles.push_back({
            Vertex2(1, 0, 1),
            Vertex2(4, 0, 2),
            Vertex2(3, 0, 2), });
        mesh.triangles.push_back({
            Vertex2(1, 0, 1),
            Vertex2(2, 0, 1),
            Vertex2(4, 0, 2), });
        mesh.triangles.push_back({
            Vertex2(3, 0, 2),
            Vertex2(6, 0, 3),
            Vertex2(5, 0, 3), });
        mesh.triangles.push_back({
            Vertex2(3, 0, 2),
            Vertex2(4, 0, 2),
            Vertex2(6, 0, 3), });
        mesh.triangles.push_back({
            Vertex2(5, 0, 3),
            Vertex2(8, 0, 4),
            Vertex2(7, 0, 4), });
        mesh.triangles.push_back({
            Vertex2(5, 0, 3),
            Vertex2(6, 0, 3),
            Vertex2(8, 0, 4), });
        event.render->drawColorMesh(
            mesh,
            Color4F(1.F, 1.F, 1.F));

        // Draw the handles.
        const Box2I& g = getGeometry();
        int pos = _valueToPos(getValue().min());
        mesh = TriMesh2F();
        mesh.v.push_back(V2F(pos, g.max.y + 1 - p.size.handle));
        mesh.v.push_back(V2F(pos + p.size.handle / 2, g.max.y + 1));
        mesh.v.push_back(V2F(pos - p.size.handle / 2, g.max.y + 1));
        mesh.triangles.push_back({
            Vertex2(1),
            Vertex2(3),
            Vertex2(2) });
        event.render->drawMesh(
            mesh,
            event.style->getColorRole(ColorRole::Text));

        pos = _valueToPos(getValue().max());
        mesh = TriMesh2F();
        mesh.v.push_back(V2F(pos, g.max.y + 1 - p.size.handle));
        mesh.v.push_back(V2F(pos + p.size.handle / 2, g.max.y + 1));
        mesh.v.push_back(V2F(pos - p.size.handle / 2, g.max.y + 1));
        mesh.triangles.push_back({
            Vertex2(1),
            Vertex2(3),
            Vertex2(2) });
        event.render->drawMesh(
            mesh,
            event.style->getColorRole(ColorRole::Text));
    }

    void LevelsSlider::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void LevelsSlider::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void LevelsSlider::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (0 == p.mouse.handle)
        {
            const float min = _posToValue(_getMousePos().x);
            const float max = std::max(min, p.model->getValue().max());
            p.model->setValue(RangeF(min, max));
        }
        else if (1 == p.mouse.handle)
        {
            const float max = _posToValue(_getMousePos().x);
            const float min = std::min(p.model->getValue().min(), max);
            p.model->setValue(RangeF(min, max));
        }
    }

    void LevelsSlider::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (contains(_getMinHandleGeometry(), event.pos))
        {
            p.mouse.handle = 0;
        }
        else if (contains(_getMaxHandleGeometry(), event.pos))
        {
            p.mouse.handle = 1;
        }
        setDrawUpdate();
    }

    void LevelsSlider::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        p.mouse.handle = -1;
        if (p.pressedCallback && !p.blockCallbacks)
        {
            p.pressedCallback(p.model->getValue(), false);
        }
        setDrawUpdate();
    }

    float LevelsSlider::_posToValue(int pos) const
    {
        FTK_P();
        const Box2I g = _getSliderGeometry();
        const float v = (pos - g.min.x) / static_cast<float>(g.w());
        const RangeF& range = p.model->getRange();
        const float out = range.min() + (range.max() - range.min()) * v;
        return out;
    }

    int LevelsSlider::_valueToPos(float value) const
    {
        FTK_P();
        const RangeF& range = p.model->getRange();
        float v = 0.F;
        if (range.min() != range.max())
        {
            v = (value - range.min()) /
                static_cast<float>(range.max() - range.min());
        }
        const Box2I g = _getSliderGeometry();
        return g.min.x + g.w() * v;
    }

    Box2I LevelsSlider::_getSliderGeometry() const
    {
        return _p->size.g2;
    }

    Box2I LevelsSlider::_getMinHandleGeometry() const
    {
        FTK_P();
        const int pos = _valueToPos(getValue().min());
        const Box2I& g = getGeometry();
        return Box2I(
            pos - p.size.handle,
            g.min.y,
            p.size.handle * 2,
            g.h());
    }

    Box2I LevelsSlider::_getMaxHandleGeometry() const
    {
        FTK_P();
        const int pos = _valueToPos(getValue().max());
        const Box2I& g = getGeometry();
        return Box2I(
            pos - p.size.handle,
            g.min.y,
            p.size.handle * 2,
            g.h());
    }

    struct LevelsEditSlider::Private
    {
        std::shared_ptr<LevelsModel> model;

        std::shared_ptr<LevelsSlider> slider;
        std::shared_ptr<FloatEdit> minEdit;
        std::shared_ptr<FloatEdit> maxEdit;
        std::shared_ptr<ToolButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(const RangeF&)> callback;
        std::function<void(const RangeF&, bool)> pressedCallback;
        int blockCallbacks = 0;

        std::shared_ptr<Observer<RangeF> > valueObserver;
        std::shared_ptr<Observer<RangeF> > rangeObserver;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;
        std::shared_ptr<Observer<RangeF> > defaultObserver;
    };

    void LevelsEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::LevelsEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = LevelsModel::create();

        p.slider = LevelsSlider::create(context, p.model);
        p.slider->setHStretch(Stretch::Expanding);

        p.minEdit = FloatEdit::create(context);
        p.minEdit->setRange(p.model->getRange());

        p.maxEdit = FloatEdit::create(context);
        p.maxEdit->setRange(p.model->getRange());

        p.resetButton = ToolButton::create(context);
        p.resetButton->setIcon("Reset");
        p.resetButton->setTooltip("Reset to the default value");

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.minEdit->setParent(p.layout);
        p.slider->setParent(p.layout);
        p.maxEdit->setParent(p.layout);
        p.resetButton->setParent(p.layout);

        p.slider->setCallback(
            [this](const RangeF& value)
            {
                FTK_P();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.slider->setPressedCallback(
            [this](const RangeF& value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, pressed);
                }
            });

        p.minEdit->setCallback(
            [this](float value)
            {
                FTK_P();
                const float max = std::max(value, p.model->getValue().max());
                p.model->setValue(RangeF(value, max));
            });

        p.maxEdit->setCallback(
            [this](float value)
            {
                FTK_P();
                const float min = std::min(p.model->getValue().min(), value);
                p.model->setValue(RangeF(min, value));
            });

        p.resetButton->setClickedCallback(
            [this]
            {
                _p->model->setDefault();
            });

        p.valueObserver = Observer<RangeF>::create(
            p.model->observeValue(),
            [this](const RangeF& value)
            {
                FTK_P();
                ++(p.blockCallbacks);
                p.minEdit->setValue(value.min());
                p.maxEdit->setValue(value.max());
                --(p.blockCallbacks);
                _widgetUpdate();
            });

        p.rangeObserver = Observer<RangeF>::create(
            p.model->observeRange(),
            [this](const RangeF& value)
            {
                FTK_P();
                ++(p.blockCallbacks);
                p.minEdit->setRange(value);
                p.maxEdit->setRange(value);
                --(p.blockCallbacks);
            });

        p.hasDefaultObserver = Observer<bool>::create(
            p.model->observeHasDefault(),
            [this](bool)
            {
                _widgetUpdate();
            });

        p.defaultObserver = Observer<RangeF>::create(
            p.model->observeDefault(),
            [this](const RangeF&)
            {
                _widgetUpdate();
            });
    }

    LevelsEditSlider::LevelsEditSlider() :
        _p(new Private)
    {}

    LevelsEditSlider::~LevelsEditSlider()
    {}

    std::shared_ptr<LevelsEditSlider> LevelsEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<LevelsEditSlider>(new LevelsEditSlider);
        out->_init(context, parent);
        return out;
    }

    const RangeF& LevelsEditSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void LevelsEditSlider::setValue(const RangeF& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void LevelsEditSlider::setCallback(const std::function<void(const RangeF&)>& value)
    {
        _p->callback = value;
    }

    void LevelsEditSlider::setPressedCallback(const std::function<void(const RangeF&, bool) > & value)
    {
        _p->pressedCallback = value;
    }

    const RangeF& LevelsEditSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void LevelsEditSlider::setRange(const RangeF& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void LevelsEditSlider::setRange(float min, float max)
    {
        setRange(RangeF(min, max));
    }

    const std::shared_ptr<LevelsModel>& LevelsEditSlider::getModel() const
    {
        return _p->model;
    }

    int LevelsEditSlider::getPrecision() const
    {
        return _p->minEdit->getPrecision();
    }

    void LevelsEditSlider::setPrecision(int value)
    {
        _p->minEdit->setPrecision(value);
        _p->maxEdit->setPrecision(value);
    }

    FontRole LevelsEditSlider::getFontRole() const
    {
        return _p->minEdit->getFontRole();
    }

    void LevelsEditSlider::setFontRole(FontRole value)
    {
        _p->minEdit->setFontRole(value);
        _p->maxEdit->setFontRole(value);
    }
    
    Size2I LevelsEditSlider::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void LevelsEditSlider::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void LevelsEditSlider::_widgetUpdate()
    {
        FTK_P();
        p.resetButton->setVisible(p.model->hasDefault());
        p.resetButton->setEnabled(p.model->getValue() != p.model->getDefault());
    }
}
