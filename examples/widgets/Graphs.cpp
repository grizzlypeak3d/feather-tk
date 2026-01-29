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
        auto vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["Images"] = Label::create(context, vLayout);
        _graphs["Images"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["ImagesSize"] = Label::create(context, vLayout);
        _graphs["ImagesSize"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["GL"] = Label::create(context, vLayout);
        _graphs["GL"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["GLSize"] = Label::create(context, vLayout);
        _graphs["GLSize"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["Widgets"] = Label::create(context, vLayout);
        _graphs["Widgets"] = GraphWidget::create(context, vLayout);

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
        const size_t images = Image::getObjectCount();
        _labels["Images"]->setText(Format("Images: {0}").arg(images));
        _graphs["Images"]->addSample(images);

        const size_t imagesSize = Image::getTotalByteCount() / megabyte;
        _labels["ImagesSize"]->setText(Format("Images size: {0}MB").arg(imagesSize));
        _graphs["ImagesSize"]->addSample(imagesSize);

        const size_t meshes = gl::VBO::getObjectCount();
        const size_t textures = gl::Texture::getObjectCount();
        const size_t buffers = gl::OffscreenBuffer::getObjectCount();
        const size_t shaders = gl::Shader::getObjectCount();
        _labels["GL"]->setText(
            Format("Meshes: {0}, textures: {1}, buffers: {2}, shaders: {3}").
            arg(meshes).
            arg(textures).
            arg(buffers).
            arg(shaders));
        _graphs["GL"]->addSample(meshes);
        _graphs["GL"]->addSample(textures, ColorRole::Red);
        _graphs["GL"]->addSample(buffers, ColorRole::Green);
        _graphs["GL"]->addSample(shaders, ColorRole::Yellow);

        const size_t meshesSize = gl::VBO::getTotalByteCount() / megabyte;
        const size_t texturesSize = gl::Texture::getTotalByteCount() / megabyte;
        _labels["GLSize"]->setText(
            Format("Meshes size: {0}MB, textures size: {1}MB").
            arg(meshesSize).
            arg(texturesSize));
        _graphs["GLSize"]->addSample(meshesSize);
        _graphs["GLSize"]->addSample(texturesSize, ColorRole::Red);

        const size_t widgets = IWidget::getObjectCount();
        _labels["Widgets"]->setText(Format("Widgets: {0}").arg(widgets));
        _graphs["Widgets"]->addSample(widgets);
    }
}
