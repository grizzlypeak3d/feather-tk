// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Graphs.h"

#include <ftk/UI/RowLayout.h>

#include <ftk/gl/Mesh.h>
#include <ftk/gl/OffscreenBuffer.h>
#include <ftk/gl/Shader.h>
#include <ftk/gl/Texture.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Graphs::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Graphs", nullptr);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create graph widgets.
        const std::vector<std::string> graphs =
        {
            "Images",
            "ImagesSize",
            "Meshes",
            "MeshesSize",
            "Textures",
            "TexturesSize",
            "OffscreenBuffers",
            "Shaders",
            "Widgets"
        };
        for (const auto& graph : graphs)
        {
            auto vLayout = VerticalLayout::create(context, layout);
            layout->setSpacingRole(SizeRole::SpacingSmall);
            _labels[graph] = Label::create(context, vLayout);
            _graphs[graph] = GraphWidget::create(context, vLayout);
        }

        _widgetUpdate();

        _timer = Timer::create(context);
        _timer->setRepeating(true);
        _timer->start(
            std::chrono::milliseconds(1000),
            [this]
            {
                _widgetUpdate();
            });
    }

    Graphs::~Graphs()
    {}

    std::shared_ptr<Graphs> Graphs::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Graphs>(new Graphs);
        out->_init(context);
        return out;
    }

    Size2I Graphs::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Graphs::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }

    void Graphs::_widgetUpdate()
    {
        size_t count = Image::getObjectCount();
        _labels["Images"]->setText(Format("Images: {0}").arg(count));
        _graphs["Images"]->addSample(count);

        count = Image::getTotalByteCount() / megabyte;
        _labels["ImagesSize"]->setText(Format("Total images size: {0}MB").arg(count));
        _graphs["ImagesSize"]->addSample(count);

        count = gl::VBO::getObjectCount();
        _labels["Meshes"]->setText(Format("Meshes: {0}").arg(count));
        _graphs["Meshes"]->addSample(count);

        count = gl::VBO::getTotalByteCount() / megabyte;
        _labels["MeshesSize"]->setText(Format("Total meshes size: {0}MB").arg(count));
        _graphs["MeshesSize"]->addSample(count);

        count = gl::Texture::getObjectCount();
        _labels["Textures"]->setText(Format("Textures: {0}").arg(count));
        _graphs["Textures"]->addSample(count);

        count = gl::Texture::getTotalByteCount() / megabyte;
        _labels["TexturesSize"]->setText(Format("Total textures size: {0}MB").arg(count));
        _graphs["TexturesSize"]->addSample(count);

        count = gl::OffscreenBuffer::getObjectCount();
        _labels["OffscreenBuffers"]->setText(Format("Offscreen buffers: {0}").arg(count));
        _graphs["OffscreenBuffers"]->addSample(count);

        count = gl::OffscreenBuffer::getObjectCount();
        _labels["Shaders"]->setText(Format("Shaders: {0}").arg(count));
        _graphs["Shaders"]->addSample(count);

        count = IWidget::getObjectCount();
        _labels["Widgets"]->setText(Format("Widgets: {0}").arg(count));
        _graphs["Widgets"]->addSample(count);
    }
}
