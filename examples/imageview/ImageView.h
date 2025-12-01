// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <ftk/Core/RenderOptions.h>

namespace imageview
{
    class App;
    class Document;

    //! Image view widget.
    class ImageView : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<Document>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        ImageView() = default;

    public:
        virtual ~ImageView();

        //! Create a new view.
        static std::shared_ptr<ImageView> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<Document>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

        //! \name Zoom
        ///@{

        float getZoom() const;
        std::shared_ptr<ftk::IObservable<float> > observeZoom() const;
        void setZoom(float);
        void frame();
        void zoomReset();
        void zoomIn();
        void zoomOut();

        ///@}

        //! \name Channel Display
        ///@{

        ftk::ChannelDisplay getChannelDisplay() const;
        std::shared_ptr<ftk::IObservable<ftk::ChannelDisplay> > observeChannelDisplay() const;
        void setChannelDisplay(ftk::ChannelDisplay);

        ///@}

        void setGeometry(const ftk::Box2I&) override;
        void sizeHintEvent(const ftk::SizeHintEvent&) override;
        void drawEvent(const ftk::Box2I&, const ftk::DrawEvent&) override;

    private:
        std::shared_ptr<ftk::Image> _image;
        std::shared_ptr<ftk::Observable<float> > _zoom;
        bool _frameInit = true;
        std::shared_ptr<ftk::Observable<ftk::ChannelDisplay> > _channelDisplay;
    };
}
