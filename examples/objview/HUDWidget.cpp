// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "HUDWidget.h"

#include "App.h"
#include "Document.h"

#include <ftk/UI/Spacer.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

using namespace ftk;

namespace objview
{
    void HUDWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IDocument>& idoc,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::objview::HUDWidget", parent);

        // Create the labels.
        for (const auto& key : { "Name", "Triangles", "Orbit", "Distance" })
        {
            _labels[key] = Label::create(context);
            _labels[key]->setMarginRole(SizeRole::MarginInside);
            _labels[key]->setBackgroundRole(ColorRole::Overlay);
        }

        if (auto doc = std::dynamic_pointer_cast<Document>(idoc))
        {
            _labels["Name"]->setText(Format("Path: {0}").arg(doc->getPath().u8string()));
            auto mesh = doc->getMesh();
            _labels["Triangles"]->setText(Format("Triangles: {0}").arg(mesh ? mesh->triangles.size() : 0));
        }

        // Layout the widgets.
        _layout = GridLayout::create(context, shared_from_this());
        _layout->setMarginRole(SizeRole::MarginSmall);
        _layout->setSpacingRole(SizeRole::SpacingSmall);

        _labels["Name"]->setParent(_layout);
        _layout->setGridPos(_labels["Name"], 0, 0);
        _labels["Triangles"]->setParent(_layout);
        _layout->setGridPos(_labels["Triangles"], 0, 2);

        _labels["Orbit"]->setParent(_layout);
        _layout->setGridPos(_labels["Orbit"], 2, 0);
        _labels["Distance"]->setParent(_layout);
        _layout->setGridPos(_labels["Distance"], 2, 2);

        // Use an exanding spacer for the center widget.
        auto spacer = Spacer::create(context, Orientation::Horizontal, _layout);
        spacer->setStretch(Stretch::Expanding);
        _layout->setGridPos(spacer, 1, 1);
    }

    HUDWidget::~HUDWidget()
    {}

    std::shared_ptr<HUDWidget> HUDWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IDocument>& doc,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<HUDWidget>(new HUDWidget);
        out->_init(context, doc, parent);
        return out;
    }

    void HUDWidget::setOrbit(const V2F& value)
    {
        _labels["Orbit"]->setText(Format("Orbit: {0} {1}").arg(value.x, 2).arg(value.y, 2));
    }

    void HUDWidget::setDistance(float value)
    {
        _labels["Distance"]->setText(Format("Distance: {0}").arg(value, 2));
    }

    void HUDWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void HUDWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_layout->getSizeHint());
    }
}
