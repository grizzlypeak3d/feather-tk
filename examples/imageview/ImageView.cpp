// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "ImageView.h"

#include "App.h"
#include "Document.h"
#include "SettingsModel.h"

#include <ftk/UI/ScrollArea.h>

#include <cmath>

using namespace ftk;

namespace imageview
{
    void ImageView::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<Document>& doc,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::imageview::ImageView", parent);

        _image = doc->getImage();
        _zoom = Observable<float>::create(1.F);
        _channelDisplay = Observable<ChannelDisplay>::create(ChannelDisplay::Color);
    }

    ImageView::~ImageView()
    {}

    std::shared_ptr<ImageView> ImageView::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<Document>& doc,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ImageView>(new ImageView);
        out->_init(context, app, doc, parent);
        return out;
    }

    float ImageView::getZoom() const
    {
        return _zoom->get();
    }

    std::shared_ptr<ftk::IObservable<float> > ImageView::observeZoom() const
    {
        return _zoom;
    }

    void ImageView::setZoom(float value)
    {
        if (_zoom->setIfChanged(value))
        {
            // A state change a widget draws or sizes from must set the
            // update flags itself; a missed flag fails silently as a
            // stale picture.
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    void ImageView::frame()
    {
        auto scrollArea = getParentT<ScrollArea>();
        if (_image && scrollArea)
        {
            const float imageAspect = _image->getAspect();
            const Box2I& g = scrollArea->getGeometry();
            const float viewAspect = aspectRatio(g.size());
            setZoom(
                imageAspect >= viewAspect ?
                g.w() / static_cast<float>(_image->getWidth()) :
                g.h() / static_cast<float>(_image->getHeight()));
        }
    }

    void ImageView::zoomReset()
    {
        setZoom(1.F);
    }

    void ImageView::zoomIn()
    {
        setZoom(_zoom->get() * 1.1F);
    }

    void ImageView::zoomOut()
    {
        setZoom(_zoom->get() * .9F);
    }

    ftk::ChannelDisplay ImageView::getChannelDisplay() const
    {
        return _channelDisplay->get();
    }

    std::shared_ptr<ftk::IObservable<ftk::ChannelDisplay> > ImageView::observeChannelDisplay() const
    {
        return _channelDisplay;
    }

    void ImageView::setChannelDisplay(ftk::ChannelDisplay value)
    {
        if (_channelDisplay->setIfChanged(value))
        {
            setDrawUpdate();
        }
    }

    // A custom widget is a size hint plus a draw: the size hint is the
    // zoomed image, so the scroll area around this widget provides the
    // panning, and the draw paints whatever the current state says.
    Size2I ImageView::getSizeHint() const
    {
        Size2I out;
        if (_image)
        {
            out = _image->getSize() * _zoom->get();
        }
        return out;
    }

    void ImageView::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        // Framing needs the scroll area's laid out size, so the first
        // frame waits for the first geometry instead of running at
        // construction.
        if (_frameInit)
        {
            _frameInit = false;
            frame();
        }
    }

    void ImageView::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);

        const Box2I& g = getGeometry();
        event.render->drawRect(g, Color4F(0.F, 0.F, 0.F));

        if (_image)
        {
            const Size2I& size = _image->getSize() * _zoom->get();
            ImageOptions options;
            //options.videoLevels = InputVideoLevels::LegalRange;
            options.channelDisplay = _channelDisplay->get();
            event.render->drawImage(
                _image,
                Box2I(
                    g.x() + g.w() / 2 - size.w / 2,
                    g.y() + g.h() / 2 - size.h / 2,
                    size.w,
                    size.h),
                Color4F(1.F, 1.F, 1.F),
                options);
        }
    }

    // Two fingers pinch the zoom and drag the scroll area. The point of
    // the image between the fingers stays between them, so the scroll
    // position is worked out here, with the zoom, rather than left to the
    // scroll widget around the view.
    void ImageView::gestureEvent(GestureEvent& event)
    {
        IWidget::gestureEvent(event);
        auto scrollArea = getParentT<ScrollArea>();
        if (_image && scrollArea)
        {
            event.accept = true;

            // What was left over belongs to a scroll position the view is
            // no longer at if it was scrolled some other way since.
            const V2I scrollPosPrev = scrollArea->getScrollPos();
            if (scrollPosPrev != _gestureScrollPos)
            {
                _gestureRemainder = V2F();
            }

            // Where the view is, worked out from the zoom and the scroll
            // position rather than read from its geometry: each finger
            // sends its own events, several arrive before the next layout,
            // and the geometry is the last layout's. The scroll area sizes
            // the view to the larger of the image and itself, and the
            // image is centered in the view.
            const float zoom = _zoom->get();
            const Box2I& area = scrollArea->getGeometry();
            const auto viewSize = [this, &area](float zoom)
            {
                const Size2I imageSize = _image->getSize() * zoom;
                return Size2I(
                    std::max(imageSize.w, area.w()),
                    std::max(imageSize.h, area.h()));
            };
            const auto imageMin = [this, &area](float zoom, const Size2I& viewSize)
            {
                const Size2I imageSize = _image->getSize() * zoom;
                return V2I(
                    area.x() + viewSize.w / 2 - imageSize.w / 2,
                    area.y() + viewSize.h / 2 - imageSize.h / 2);
            };

            // The image point that was between the fingers.
            const V2I imageMinPrev = imageMin(zoom, viewSize(zoom)) - scrollPosPrev;
            const V2F prev(event.pos.x - event.pan.x, event.pos.y - event.pan.y);
            const V2F imagePos(
                (prev.x - imageMinPrev.x + _gestureRemainder.x) / zoom,
                (prev.y - imageMinPrev.y + _gestureRemainder.y) / zoom);

            // The scroll position that puts it back between them at the new
            // zoom, kept inside what the scroll area can scroll: an image
            // that fits has nowhere to go, and a position past the edge
            // drew for a frame and then snapped back at the next layout.
            const float zoomNew = clamp(zoom * event.zoom, .01F, 100.F);
            const Size2I viewSizeNew = viewSize(zoomNew);
            const V2I imageMinNew = imageMin(zoomNew, viewSizeNew);
            V2F scrollPos(
                imageMinNew.x + imagePos.x * zoomNew - event.pos.x,
                imageMinNew.y + imagePos.y * zoomNew - event.pos.y);
            scrollPos.x = clamp(scrollPos.x, 0.F, static_cast<float>(std::max(0, viewSizeNew.w - area.w())));
            scrollPos.y = clamp(scrollPos.y, 0.F, static_cast<float>(std::max(0, viewSizeNew.h - area.h())));

            // What is left over from rounding carries to the next event,
            // or fingers moving slowly would move nothing.
            const V2I scrollPosI(std::round(scrollPos.x), std::round(scrollPos.y));
            _gestureRemainder = V2F(scrollPos.x - scrollPosI.x, scrollPos.y - scrollPosI.y);
            _gestureScrollPos = scrollPosI;
            setZoom(zoomNew);
            scrollArea->setScrollPos(scrollPosI, false);
        }
    }
}
