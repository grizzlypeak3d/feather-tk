// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorSlider.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct ColorIntSlider::Private
    {
        std::vector<V4F> colors;

        struct SizeData
        {
            bool init = true;
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            TriMesh2F border;
            TriMesh2F keyFocus;
        };
        std::optional<DrawData> draw;
    };

    void ColorIntSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IIntSlider::_init(context, model, parent);
        FTK_P();
    }

    ColorIntSlider::ColorIntSlider() :
        _p(new Private)
    {}

    ColorIntSlider::~ColorIntSlider()
    {}

    std::shared_ptr<ColorIntSlider> ColorIntSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ColorIntSlider>(new ColorIntSlider);
        out->_init(context, model, parent);
        return out;
    }

    void ColorIntSlider::setColors(const std::vector<V4F>& value)
    {
        FTK_P();
        if (value == p.colors)
            return;
        p.colors = value;
        setDrawUpdate();
    }

    Size2I ColorIntSlider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void ColorIntSlider::setGeometry(const Box2I& value)
    {
        if (value != getGeometry())
        {
            _p->draw.reset();
        }
        IIntSlider::setGeometry(value);
    }

    void ColorIntSlider::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        IIntSlider::styleEvent(event);
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void ColorIntSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IIntSlider::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFont(FontType::Regular, event.displayScale);
            p.size.sizeHint = Size2I(p.size.size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.keyFocus);

            p.draw.reset();
        }
    }

    void ColorIntSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IIntSlider::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }
    
    void ColorIntSlider::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = _getInsideGeometry();
            p.draw->g2 = _getSliderGeometry();
            p.draw->border = border(margin(p.draw->g2, p.size.border), p.size.border);
            p.draw->keyFocus = border(p.draw->g, p.size.keyFocus);
        }

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Draw the colors.
        TriMesh2F mesh;
        int w = p.draw->g2.w();
        int h = p.draw->g2.h();
        for (size_t i = 0; i < p.colors.size(); ++i)
        {
            const int x = p.draw->g2.min.x + i / (static_cast<float>(p.colors.size() - 1)) * w;
            mesh.v.push_back(V2F(x, p.draw->g2.min.y));
            mesh.v.push_back(V2F(x, p.draw->g2.max.y + 1));
            mesh.c.push_back(p.colors[i]);
        }
        for (size_t i = 0; i < p.colors.size(); ++i)
        {
            mesh.triangles.push_back({
                Vertex2(i * 2 + 1, 0, i + 1),
                Vertex2(i * 2 + 2, 0, i + 1),
                Vertex2(i * 2 + 4, 0, i + 2), });
            mesh.triangles.push_back({
                Vertex2(i * 2 + 4, 0, i + 2),
                Vertex2(i * 2 + 3, 0, i + 2),
                Vertex2(i * 2 + 1, 0, i + 1), });
        }
        event.render->drawColorMesh(
            mesh,
            Color4F(1.F, 1.F, 1.F));

        // Draw the handle.
        const Box2I& g = getGeometry();
        const int pos = _valueToPos(getValue());
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

    Box2I ColorIntSlider::_getSliderGeometry() const
    {
        FTK_P();
        return margin(_getInsideGeometry(), -p.size.keyFocus);
    }

    Box2I ColorIntSlider::_getInsideGeometry() const
    {
        FTK_P();
        return margin(getGeometry(), -p.size.handle / 2, 0, -p.size.handle / 2, -p.size.handle);
    }

    struct ColorIntEditSlider::Private
    {
        std::shared_ptr<IntModel> model;

        std::shared_ptr<IntEdit> edit;
        std::shared_ptr<ColorIntSlider> slider;
        std::shared_ptr<IntResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(int)> callback;
        std::function<void(int, bool)> pressedCallback;
        int blockCallbacks = 0;
    };

    void ColorIntEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ColorIntEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = IntEdit::create(context, p.model);

        p.slider = ColorIntSlider::create(context, p.model);

        p.resetButton = IntResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.slider->setParent(p.layout);
        p.resetButton->setParent(p.layout);

        p.slider->setCallback(
            [this](int value)
            {
                FTK_P();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.slider->setPressedCallback(
            [this](int value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, pressed);
                }
            });
    }

    ColorIntEditSlider::ColorIntEditSlider() :
        _p(new Private)
    {}

    ColorIntEditSlider::~ColorIntEditSlider()
    {}

    std::shared_ptr<ColorIntEditSlider> ColorIntEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        std::shared_ptr<ColorIntEditSlider> out(new ColorIntEditSlider);
        out->_init(context, IntModel::create(), parent);
        return out;
    }

    std::shared_ptr<ColorIntEditSlider> ColorIntEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        std::shared_ptr<ColorIntEditSlider> out(new ColorIntEditSlider);
        out->_init(context, model, parent);
        return out;
    }

    const std::shared_ptr<IntModel>& ColorIntEditSlider::getModel() const
    {
        return _p->model;
    }

    void ColorIntEditSlider::setColors(const std::vector<V4F>& value)
    {
        _p->slider->setColors(value);
    }

    int ColorIntEditSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void ColorIntEditSlider::setValue(int value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void ColorIntEditSlider::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    void ColorIntEditSlider::setPressedCallback(const std::function<void(int, bool) >& value)
    {
        _p->pressedCallback = value;
    }

    const RangeI& ColorIntEditSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void ColorIntEditSlider::setRange(const RangeI& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void ColorIntEditSlider::setRange(int min, int max)
    {
        setRange(RangeI(min, max));
    }

    Size2I ColorIntEditSlider::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void ColorIntEditSlider::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    struct ColorFloatSlider::Private
    {
        std::vector<V4F> colors;

        struct SizeData
        {
            bool init = true;
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            TriMesh2F border;
            TriMesh2F keyFocus;
        };
        std::optional<DrawData> draw;
    };

    void ColorFloatSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IFloatSlider::_init(context, model, parent);
        FTK_P();
    }

    ColorFloatSlider::ColorFloatSlider() :
        _p(new Private)
    {}

    ColorFloatSlider::~ColorFloatSlider()
    {}

    std::shared_ptr<ColorFloatSlider> ColorFloatSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ColorFloatSlider>(new ColorFloatSlider);
        out->_init(context, model, parent);
        return out;
    }

    void ColorFloatSlider::setColors(const std::vector<V4F>& value)
    {
        FTK_P();
        if (value == p.colors)
            return;
        p.colors = value;
        setDrawUpdate();
    }

    Size2I ColorFloatSlider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void ColorFloatSlider::setGeometry(const Box2I& value)
    {
        if (value != getGeometry())
        {
            _p->draw.reset();
        }
        IFloatSlider::setGeometry(value);
    }

    void ColorFloatSlider::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        IFloatSlider::styleEvent(event);
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void ColorFloatSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IFloatSlider::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFont(FontType::Regular, event.displayScale);
            p.size.sizeHint = Size2I(p.size.size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.keyFocus);

            p.draw.reset();
        }
    }

    void ColorFloatSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IFloatSlider::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }
    
    void ColorFloatSlider::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = _getInsideGeometry();
            p.draw->g2 = _getSliderGeometry();
            p.draw->border = border(margin(p.draw->g2, p.size.border), p.size.border);
            p.draw->keyFocus = border(p.draw->g, p.size.keyFocus);
        }

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Draw the colors.
        TriMesh2F mesh;
        int w = p.draw->g2.w();
        int h = p.draw->g2.h();
        for (size_t i = 0; i < p.colors.size(); ++i)
        {
            const int x = p.draw->g2.min.x + i / (static_cast<float>(p.colors.size() - 1)) * w;
            mesh.v.push_back(V2F(x, p.draw->g2.min.y));
            mesh.v.push_back(V2F(x, p.draw->g2.max.y + 1));
            mesh.c.push_back(p.colors[i]);
        }
        for (size_t i = 0; i < p.colors.size(); ++i)
        {
            mesh.triangles.push_back({
                Vertex2(i * 2 + 1, 0, i + 1),
                Vertex2(i * 2 + 2, 0, i + 1),
                Vertex2(i * 2 + 4, 0, i + 2), });
            mesh.triangles.push_back({
                Vertex2(i * 2 + 4, 0, i + 2),
                Vertex2(i * 2 + 3, 0, i + 2),
                Vertex2(i * 2 + 1, 0, i + 1), });
        }
        event.render->drawColorMesh(
            mesh,
            Color4F(1.F, 1.F, 1.F));

        // Draw the handle.
        const Box2I& g = getGeometry();
        const int pos = _valueToPos(getValue());
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

    Box2I ColorFloatSlider::_getSliderGeometry() const
    {
        FTK_P();
        return margin(_getInsideGeometry(), -p.size.keyFocus);
    }

    Box2I ColorFloatSlider::_getInsideGeometry() const
    {
        FTK_P();
        return margin(getGeometry(), -p.size.handle / 2, 0, -p.size.handle / 2, -p.size.handle);
    }

    struct ColorFloatEditSlider::Private
    {
        std::shared_ptr<FloatModel> model;

        std::shared_ptr<FloatEdit> edit;
        std::shared_ptr<ColorFloatSlider> slider;
        std::shared_ptr<FloatResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(float)> callback;
        std::function<void(float, bool)> pressedCallback;
        int blockCallbacks = 0;
    };

    void ColorFloatEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ColorFloatEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = FloatEdit::create(context, p.model);

        p.slider = ColorFloatSlider::create(context, p.model);

        p.resetButton = FloatResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.slider->setParent(p.layout);
        p.resetButton->setParent(p.layout);

        p.slider->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.slider->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, pressed);
                }
            });
    }

    ColorFloatEditSlider::ColorFloatEditSlider() :
        _p(new Private)
    {}

    ColorFloatEditSlider::~ColorFloatEditSlider()
    {}

    std::shared_ptr<ColorFloatEditSlider> ColorFloatEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        std::shared_ptr<ColorFloatEditSlider> out(new ColorFloatEditSlider);
        out->_init(context, FloatModel::create(), parent);
        return out;
    }

    std::shared_ptr<ColorFloatEditSlider> ColorFloatEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        std::shared_ptr<ColorFloatEditSlider> out(new ColorFloatEditSlider);
        out->_init(context, model, parent);
        return out;
    }

    const std::shared_ptr<FloatModel>& ColorFloatEditSlider::getModel() const
    {
        return _p->model;
    }

    void ColorFloatEditSlider::setColors(const std::vector<V4F>& value)
    {
        _p->slider->setColors(value);
    }

    float ColorFloatEditSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void ColorFloatEditSlider::setValue(float value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void ColorFloatEditSlider::setCallback(const std::function<void(float)>& value)
    {
        _p->callback = value;
    }

    void ColorFloatEditSlider::setPressedCallback(const std::function<void(float, bool) >& value)
    {
        _p->pressedCallback = value;
    }

    const RangeF& ColorFloatEditSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void ColorFloatEditSlider::setRange(const RangeF& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void ColorFloatEditSlider::setRange(float min, float max)
    {
        setRange(RangeF(min, max));
    }

    Size2I ColorFloatEditSlider::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void ColorFloatEditSlider::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}
