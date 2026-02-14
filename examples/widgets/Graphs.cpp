// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Graphs.h"

#include <ftk/UI/RowLayout.h>

#include <ftk/GL/Mesh.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Shader.h>
#include <ftk/GL/Texture.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Graphs::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Graphs", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create graph widgets.
        auto vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _graphs["GLObjects"] = GraphWidget::create(
            context,
            "OpenGL Objects",
            {
                { ColorRole::Cyan, "Meshes: {0}" },
                { ColorRole::Magenta, "Textures: {0}" },
                { ColorRole::Yellow, "Buffers: {0}" },
                { ColorRole::Red, "Shaders: {0}" }
            },
            vLayout);
        _graphs["GLMemory"] = GraphWidget::create(
            context,
            "OpenGL Memory",
            {
                { ColorRole::Cyan, "Meshes: {0}MB" },
                { ColorRole::Magenta, "Textures: {0}MB" }
            },
            vLayout);
        _graphs["Objects"] = GraphWidget::create(
            context,
            "Objects",
            {
                { ColorRole::Cyan, "Images: {0}" },
                { ColorRole::Magenta, "Widgets: {0}" }
            },
            vLayout);
        _graphs["Memory"] = GraphWidget::create(
            context,
            "Memory",
            {
                { ColorRole::Cyan, "Images: {0}MB" }
            },
            vLayout);

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

    std::shared_ptr<IWidget> Graphs::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Graphs>(new Graphs);
        out->_init(context, parent);
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
        _graphs["GLObjects"]->addSample(ColorRole::Cyan, gl::VBO::getObjectCount());
        _graphs["GLObjects"]->addSample(ColorRole::Magenta, gl::Texture::getObjectCount());
        _graphs["GLObjects"]->addSample(ColorRole::Yellow, gl::OffscreenBuffer::getObjectCount());
        _graphs["GLObjects"]->addSample(ColorRole::Red, gl::Shader::getObjectCount());

        _graphs["GLMemory"]->addSample(ColorRole::Cyan, gl::VBO::getTotalByteCount() / megabyte);
        _graphs["GLMemory"]->addSample(ColorRole::Magenta, gl::Texture::getTotalByteCount() / megabyte);

        _graphs["Objects"]->addSample(ColorRole::Cyan, Image::getObjectCount());
        _graphs["Objects"]->addSample(ColorRole::Magenta, IWidget::getObjectCount());

        _graphs["Memory"]->addSample(ColorRole::Cyan, Image::getTotalByteCount() / megabyte);
    }
}
