// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/BoxPack.h>

#include <algorithm>

namespace ftk
{
    bool BoxPackNode::isBranch() const
    {
        return children[0].get();
    }

    bool BoxPackNode::isOccupied() const
    {
        return id != boxPackInvalidID;
    }

    std::shared_ptr<BoxPack> BoxPack::create(const Size2I& size, int border)
    {
        auto out = std::shared_ptr<BoxPack>(new BoxPack);
        out->_border = border;
        out->_root = std::make_shared<BoxPackNode>();
        out->_root->box = Box2I(V2I(), size);
        return out;
    }

    const std::shared_ptr<BoxPackNode>& BoxPack::getRoot() const
    {
        return _root;
    }

    std::vector<std::shared_ptr<BoxPackNode> > BoxPack::getNodes() const
    {
        std::vector<std::shared_ptr<BoxPackNode> > out;
        _getNodes(_root, out);
        return out;
    }

    std::shared_ptr<BoxPackNode> BoxPack::getNode(BoxPackID id)
    {
        std::shared_ptr<BoxPackNode> out;
        if (const auto i = _idToNode.find(id); i != _idToNode.end())
        {
            i->second->timestamp = _timestamp++;
            out = i->second;
        }
        return out;
    }

    std::shared_ptr<BoxPackNode> BoxPack::insert(const Size2I& size)
    {
        auto out = _insert(_root, size);
        if (!out)
        {
            std::vector<std::shared_ptr<BoxPackNode> > nodes;
            _getNodes(_root, nodes);
            std::sort(
                nodes.begin(),
                nodes.end(),
                [](const std::shared_ptr<BoxPackNode>& a,
                    const std::shared_ptr<BoxPackNode>& b)
                {
                    return a->timestamp < b->timestamp;
                });
            const Size2I sizeAndBorder = size + _border * 2;
            for (auto node : nodes)
            {
                const Size2I nodeSize = node->box.size();
                if (sizeAndBorder.w <= nodeSize.w &&
                    sizeAndBorder.h <= nodeSize.h)
                {
                    _removeFromMap(node);
                    node->id = boxPackInvalidID;
                    node->timestamp = 0;
                    node->children[0].reset();
                    node->children[1].reset();
                    out = _insert(node, size);
                    break;
                }
            }
        }
        return out;
    }

    void BoxPack::_getNodes(
        const std::shared_ptr<BoxPackNode>& node,
        std::vector<std::shared_ptr<BoxPackNode> >& nodes) const
    {
        nodes.push_back(node);
        if (node->children[0])
        {
            _getNodes(node->children[0], nodes);
        }
        if (node->children[1])
        {
            _getNodes(node->children[1], nodes);
        }
    }

    std::shared_ptr<BoxPackNode> BoxPack::_insert(
        std::shared_ptr<BoxPackNode> node,
        const Size2I& size)
    {
        std::shared_ptr<BoxPackNode> out;
        if (node->isBranch())
        {
            node->timestamp = _timestamp++;
            out = _insert(node->children[0], size);
            if (!out)
            {
                out = _insert(node->children[1], size);
            }
        }
        else if (!node->isOccupied())
        {
            const Size2I nodeSize = node->box.size();
            const Size2I sizeAndBorder = size + _border * 2;
            if (sizeAndBorder == nodeSize)
            {
                node->id = _id++;
                node->timestamp = _timestamp++;
                _idToNode[node->id] = node;
                out = node;
            }
            else if (sizeAndBorder.w <= nodeSize.w &&
                sizeAndBorder.h <= nodeSize.h)
            {
                node->children[0] = std::make_shared<BoxPackNode>();
                node->children[1] = std::make_shared<BoxPackNode>();
                const int dw = nodeSize.w - sizeAndBorder.w;
                const int dh = nodeSize.h - sizeAndBorder.h;
                if (dw > dh)
                {
                    node->children[0]->box.min.x = node->box.min.x;
                    node->children[0]->box.min.y = node->box.min.y;
                    node->children[0]->box.max.x = node->box.min.x + sizeAndBorder.w - 1;
                    node->children[0]->box.max.y = node->box.max.y;

                    node->children[1]->box.min.x = node->box.min.x + sizeAndBorder.w;
                    node->children[1]->box.min.y = node->box.min.y;
                    node->children[1]->box.max.x = node->box.max.x;
                    node->children[1]->box.max.y = node->box.max.y;
                }
                else
                {
                    node->children[0]->box.min.x = node->box.min.x;
                    node->children[0]->box.min.y = node->box.min.y;
                    node->children[0]->box.max.x = node->box.max.x;
                    node->children[0]->box.max.y = node->box.min.y + sizeAndBorder.h - 1;

                    node->children[1]->box.min.x = node->box.min.x;
                    node->children[1]->box.min.y = node->box.min.y + sizeAndBorder.h;
                    node->children[1]->box.max.x = node->box.max.x;
                    node->children[1]->box.max.y = node->box.max.y;
                }
                out = _insert(node->children[0], size);
            }
        }
        return out;
    }

    void BoxPack::_removeFromMap(const std::shared_ptr<BoxPackNode>& node)
    {
        if (const auto i = _idToNode.find(node->id); i != _idToNode.end())
        {
            _idToNode.erase(i);
        }
        if (node->children[0])
        {
            _removeFromMap(node->children[0]);
        }
        if (node->children[1])
        {
            _removeFromMap(node->children[1]);
        }
    }
}
