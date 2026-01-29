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
        _labels["Meshes"] = Label::create(context, vLayout);
        _graphs["Meshes"] = GraphWidget::create(context, vLayout);
        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["MeshesSize"] = Label::create(context, vLayout);
        _graphs["MeshesSize"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["Textures"] = Label::create(context, vLayout);
        _graphs["Textures"] = GraphWidget::create(context, vLayout);
        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["TexturesSize"] = Label::create(context, vLayout);
        _graphs["TexturesSize"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["OffscreenBuffers"] = Label::create(context, vLayout);
        _graphs["OffscreenBuffers"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["Shaders"] = Label::create(context, vLayout);
        _graphs["Shaders"] = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _labels["Widgets"] = Label::create(context, vLayout);
        _graphs["Widgets"] = GraphWidget::create(context, vLayout);

        _timer = Timer::create(context);
        _timer->setRepeating(true);
        _timer->start(
            std::chrono::milliseconds(1000),
            [this]
            {
                size_t count = Image::getObjectCount();
                _labels["Images"]->setText(Format("Images: {0}").arg(count));
                _graphs["Images"]->addSample(count);
                count = Image::getTotalByteCount() / megabyte;
                _labels["ImagesSize"]->setText(Format("Total image size: {0}MB").arg(count));
                _graphs["ImagesSize"]->addSample(count);

                count = gl::VBO::getObjectCount();
                _labels["Meshes"]->setText(Format("Meshes: {0}").arg(count));
                _graphs["Meshes"]->addSample(count);
                count = gl::VBO::getTotalByteCount() / megabyte;
                _labels["MeshesSize"]->setText(Format("Total mesh size: {0}MB").arg(count));
                _graphs["MeshesSize"]->addSample(count);

                count = gl::Texture::getObjectCount();
                _labels["Textures"]->setText(Format("Textures: {0}").arg(count));
                _graphs["Textures"]->addSample(count);
                count = gl::Texture::getTotalByteCount() / megabyte;
                _labels["TexturesSize"]->setText(Format("Total texture size: {0}MB").arg(count));
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
}
