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
            if (scrollArea->getScrollPos() != _gestureScrollPos)
            {
                _gestureRemainder = V2F();
            }

            // The image point that was between the fingers.
            const float zoom = _zoom->get();
            const Box2I& g = getGeometry();
            const Size2I imageSize = _image->getSize() * zoom;
            const V2F prev(event.pos.x - event.pan.x, event.pos.y - event.pan.y);
            const V2F imagePos(
                (prev.x - (g.x() + g.w() / 2 - imageSize.w / 2)) / zoom,
                (prev.y - (g.y() + g.h() / 2 - imageSize.h / 2)) / zoom);

            // Where the scroll position puts it back between them at the
            // new zoom. The scroll area sizes this view to the larger of
            // the image and itself, and the image is centered in the view.
            const float zoomNew = clamp(zoom * event.zoom, .01F, 100.F);
            const Size2I imageSizeNew = _image->getSize() * zoomNew;
            const Box2I& area = scrollArea->getGeometry();
            const Size2I viewSizeNew(
                std::max(imageSizeNew.w, area.w()),
                std::max(imageSizeNew.h, area.h()));
            const V2F scrollPos(
                area.x() + viewSizeNew.w / 2 - imageSizeNew.w / 2 +
                imagePos.x * zoomNew - event.pos.x + _gestureRemainder.x * zoomNew / zoom,
                area.y() + viewSizeNew.h / 2 - imageSizeNew.h / 2 +
                imagePos.y * zoomNew - event.pos.y + _gestureRemainder.y * zoomNew / zoom);

            // What is left over from rounding carries to the next event,
            // or fingers moving slowly would move nothing.
            const V2I scrollPosI(std::round(scrollPos.x), std::round(scrollPos.y));
            _gestureRemainder = V2F(scrollPos.x - scrollPosI.x, scrollPos.y - scrollPosI.y);
            _gestureScrollPos = scrollPosI;
            setZoom(zoomNew);

            // Not clamped: the scroll size is the old zoom's until the next
            // layout, which clamps it.
            scrollArea->setScrollPos(scrollPosI, false);
        }
    }
}
