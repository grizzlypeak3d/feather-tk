// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorSlider.h>

#include <ftk/UI/DrawUtil.h>

namespace ftk
{
    struct ColorIntSlider::Private
    {
        std::vector<V4F> colors;

        struct SizeData
        {
            std::optional<float> displayScale;
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
            Box2I g;
            Box2I g2;
        };
        SizeData size;

        struct DrawData
        {
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
        const bool changed = value != getGeometry();
        IIntSlider::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.size.g = margin(value, -p.size.handle / 2, 0, -p.size.handle / 2, -p.size.handle);
            p.size.g2 = margin(p.size.g, -p.size.keyFocus);
            p.draw.reset();
        }
    }

    void ColorIntSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IIntSlider::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFontRole(FontRole::Label, event.displayScale);
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
            p.draw->border = border(margin(p.size.g2, p.size.border), p.size.border);
            p.draw->keyFocus = border(p.size.g, p.size.keyFocus);
        }

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Draw the colors.
        TriMesh2F mesh;
        int w = p.size.g2.w();
        int h = p.size.g2.h();
        for (size_t i = 0; i < p.colors.size(); ++i)
        {
            const int x = p.size.g2.min.x + i / (static_cast<float>(p.colors.size() - 1)) * w;
            mesh.v.push_back(V2F(x, p.size.g2.min.y));
            mesh.v.push_back(V2F(x, p.size.g2.max.y + 1));
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
        return _p->size.g2;
    }

    struct ColorFloatSlider::Private
    {
        std::vector<V4F> colors;

        struct SizeData
        {
            std::optional<float> displayScale;
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
            Box2I g;
            Box2I g2;
        };
        SizeData size;

        struct DrawData
        {
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
        const bool changed = value != getGeometry();
        IFloatSlider::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.size.g = margin(value, -p.size.handle / 2, 0, -p.size.handle / 2, -p.size.handle);
            p.size.g2 = margin(p.size.g, -p.size.keyFocus);
            p.draw.reset();
        }
    }

    void ColorFloatSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IFloatSlider::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFontRole(FontRole::Label, event.displayScale);
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
            p.draw->border = border(margin(p.size.g2, p.size.border), p.size.border);
            p.draw->keyFocus = border(p.size.g, p.size.keyFocus);
        }

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Draw the colors.
        TriMesh2F mesh;
        int w = p.size.g2.w();
        int h = p.size.g2.h();
        for (size_t i = 0; i < p.colors.size(); ++i)
        {
            const int x = p.size.g2.min.x + i / (static_cast<float>(p.colors.size() - 1)) * w;
            mesh.v.push_back(V2F(x, p.size.g2.min.y));
            mesh.v.push_back(V2F(x, p.size.g2.max.y + 1));
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
        return _p->size.g2;
    }
}
