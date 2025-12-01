// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "ImageView.h"

#include "App.h"
#include "Document.h"
#include "SettingsModel.h"

#include <ftk/UI/ScrollArea.h>

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

    void ImageView::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        if (_frameInit)
        {
            _frameInit = false;
            frame();
        }
    }

    void ImageView::sizeHintEvent(const SizeHintEvent& event)
    {
        Size2I sizeHint;
        if (_image)
        {
            sizeHint = _image->getSize() * _zoom->get();
        }
        setSizeHint(sizeHint);
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
}
