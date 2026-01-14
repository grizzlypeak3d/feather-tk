// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MDIMiniMap.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IMouseWidget.h>

#include <optional>

namespace ftk
{
    namespace
    {
        class FTK_API_TYPE MDIMiniMapWidget : public IMouseWidget
        {
        protected:
            void _init(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<IWidget>& parent);

            MDIMiniMapWidget() = default;

        public:
            virtual ~MDIMiniMapWidget() {}

            static std::shared_ptr<MDIMiniMapWidget> create(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<IWidget>& parent = nullptr);

            void setScrollInfo(const ScrollInfo&);
            void setCallback(const std::function<void(const V2I&)>&);
            void setChildGeometry(const std::vector<Box2I>&);
            ColorRole getChildColorRole() const;
            void setChildColorRole(ColorRole);

            FTK_API Size2I getSizeHint() const override;
            void sizeHintEvent(const SizeHintEvent&) override;
            void drawEvent(const Box2I&, const DrawEvent&) override;
            void mouseMoveEvent(MouseMoveEvent&) override;
            void mousePressEvent(MouseClickEvent&) override;

        private:
            ScrollInfo _scrollInfo;
            std::function<void(const V2I&)> _callback;
            std::vector<Box2I> _childGeometry;
            ColorRole _childColorRole = ColorRole::Cyan;

            V2I _scrollPosPress;

            struct SizeData
            {
                std::optional<float> displayScale;
                int sizeHint = 0;
                int border = 0;
            };
            SizeData _size;
        };

        void MDIMiniMapWidget::_init(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<IWidget>& parent)
        {
            IMouseWidget::_init(context, "ftk::MDIMiniMapWidget", parent);
            setBackgroundRole(ColorRole::Overlay);
            _setMouseHoverEnabled(true);
            _setMousePressEnabled(true);
        }

        std::shared_ptr<MDIMiniMapWidget> MDIMiniMapWidget::create(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<IWidget>& parent)
        {
            auto out = std::shared_ptr<MDIMiniMapWidget>(new MDIMiniMapWidget);
            out->_init(context, parent);
            return out;
        }

        void MDIMiniMapWidget::setScrollInfo(const ScrollInfo& value)
        {
            if (value == _scrollInfo)
                return;
            _scrollInfo = value;
            setSizeUpdate();
            setDrawUpdate();
        }
        
        void MDIMiniMapWidget::setCallback(const std::function<void(const V2I&)>& value)
        {
            _callback = value;
        }

        void MDIMiniMapWidget::setChildGeometry(const std::vector<Box2I>& value)
        {
            if (value == _childGeometry)
                return;
            _childGeometry = value;
            setDrawUpdate();
        }

        ColorRole MDIMiniMapWidget::getChildColorRole() const
        {
            return _childColorRole;
        }

        void MDIMiniMapWidget::setChildColorRole(ColorRole value)
        {
            if (value == _childColorRole)
                return;
            _childColorRole = value;
            setDrawUpdate();
        }
        
        Size2I MDIMiniMapWidget::getSizeHint() const
        {
            float aspect = 1.F;
            if (_scrollInfo.scrollSize.isValid())
            {
                aspect = _scrollInfo.scrollSize.w / static_cast<float>(_scrollInfo.scrollSize.h);
            }
            return Size2I(_size.sizeHint * aspect, _size.sizeHint) + _size.border * 2;
        }

        void MDIMiniMapWidget::sizeHintEvent(const SizeHintEvent& event)
        {
            IMouseWidget::sizeHintEvent(event);
            if (!_size.displayScale.has_value() ||
                (_size.displayScale.has_value() && _size.displayScale.value() != event.displayScale))
            {
                _size.displayScale = event.displayScale;
                _size.sizeHint = event.style->getSizeRole(SizeRole::ScrollArea, event.displayScale) / 2;
                _size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            }
        }

        void MDIMiniMapWidget::drawEvent(
            const Box2I& drawRect,
            const DrawEvent& event)
        {
            IMouseWidget::drawEvent(drawRect, event);

            const Box2I& g = getGeometry();
            event.render->drawMesh(
                border(g, _size.border),
                event.style->getColorRole(ColorRole::Border));

            if (_scrollInfo.scrollSize.isValid())
            {
                std::vector<Box2I> rects;
                const Box2I g2 = margin(g, -_size.border);
                const float sw = static_cast<float>(_scrollInfo.scrollSize.w);
                const float sh = static_cast<float>(_scrollInfo.scrollSize.h);
                for (const auto& wg : _childGeometry)
                {
                    rects.push_back(Box2I(
                        g2.min.x + wg.min.x / sw * g2.w(),
                        g2.min.y + wg.min.y / sh * g2.h(),
                        std::ceil(wg.w() / sw * g2.w()),
                        std::ceil(wg.h() / sh * g2.h())));
                }
                event.render->drawRects(
                    rects,
                    event.style->getColorRole(_childColorRole));

                const Box2I viewport(
                    g2.min.x + _scrollInfo.scrollPos.x / sw * g2.w(),
                    g2.min.y + _scrollInfo.scrollPos.y / sh * g2.h(),
                    std::ceil(_scrollInfo.viewport.size().w / sw * g2.w()),
                    std::ceil(_scrollInfo.viewport.size().h / sh * g2.h()));
                event.render->drawMesh(
                    border(viewport, _size.border),
                    event.style->getColorRole(ColorRole::Text));
            }
        }

        void MDIMiniMapWidget::mouseMoveEvent(MouseMoveEvent& event)
        {
            IMouseWidget::mouseMoveEvent(event);
            if (_isMousePressed() && _scrollInfo.scrollSize.isValid() && _callback)
            {
                const Box2I& g = getGeometry();
                const Box2I g2 = margin(g, -_size.border);
                const V2I& m = _getMousePos();
                const V2I& mp = _getMousePressPos();
                const V2I v(
                    (m.x - mp.x) / static_cast<float>(g2.w()) * _scrollInfo.scrollSize.w,
                    (m.y - mp.y) / static_cast<float>(g2.h()) * _scrollInfo.scrollSize.h);
                _callback(_scrollPosPress + v);
            }
        }

        void MDIMiniMapWidget::mousePressEvent(MouseClickEvent& event)
        {
            IMouseWidget::mousePressEvent(event);
            _scrollPosPress = _scrollInfo.scrollPos;
        }
    }

    struct MDIMiniMap::Private
    {
        std::shared_ptr<MDIMiniMapWidget> widget;

        struct SizeData
        {
            std::optional<float> displayScale;
            int shadow = 0;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F shadow;
        };
        std::optional<DrawData> draw;
    };

    void MDIMiniMap::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::MDIMiniMap", parent);
        FTK_P();
        p.widget = MDIMiniMapWidget::create(context, shared_from_this());
    }

    MDIMiniMap::MDIMiniMap() :
        _p(new Private)
    {}

    MDIMiniMap::~MDIMiniMap()
    {}

    std::shared_ptr<MDIMiniMap> MDIMiniMap::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<MDIMiniMap>(new MDIMiniMap);
        out->_init(context, parent);
        return out;
    }

    void MDIMiniMap::setScrollInfo(const ScrollInfo& value)
    {
        _p->widget->setScrollInfo(value);
    }

    void MDIMiniMap::setCallback(const std::function<void(const V2I&)>& value)
    {
        _p->widget->setCallback(value);
    }

    void MDIMiniMap::setChildGeometry(const std::vector<Box2I>& value)
    {
        _p->widget->setChildGeometry(value);
    }

    ColorRole MDIMiniMap::getChildColorRole() const
    {
        return _p->widget->getChildColorRole();
    }

    void MDIMiniMap::setChildColorRole(ColorRole value)
    {
        _p->widget->setChildColorRole(value);
    }

    void MDIMiniMap::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IWidget::setGeometry(value);
        FTK_P();
        const Size2I sizeHint = p.widget->getSizeHint();
        const Size2I size = margin(sizeHint, p.size.shadow);
        p.widget->setGeometry(Box2I(
            value.max.x - size.w,
            value.max.y - size.h,
            sizeHint.w,
            sizeHint.h));
        if (changed)
        {
            p.draw.reset();
        }
    }

    void MDIMiniMap::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.shadow = event.style->getSizeRole(SizeRole::Shadow, event.displayScale);
        }
    }

    void MDIMiniMap::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            const Box2I& g = p.widget->getGeometry();
            p.draw->shadow = shadow(ftk::margin(g, p.size.shadow, 0, p.size.shadow, p.size.shadow), p.size.shadow);
        }

        event.render->drawColorMesh(p.draw->shadow);
    }
}
