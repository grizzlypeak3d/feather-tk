// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Box.h>
#include <ftk/Core/Util.h>

#include <array>
#include <map>
#include <memory>
#include <vector>

namespace ftk
{
    //! \name Box Packing
    ///@{

    //! Box packing ID.
    typedef int64_t BoxPackID;

    //! Invalid box packing ID.
    constexpr BoxPackID boxPackInvalidID = -1;

    //! Box packing timestamp.
    typedef uint64_t BoxPackTimestamp;

    //! Box packing node.
    //!
    //! References:
    //! - http://blackpawn.com/texts/lightmaps/
    struct FTK_API_TYPE BoxPackNode
    {
        Box2I box;
        BoxPackID id = boxPackInvalidID;
        BoxPackTimestamp timestamp = 0;
        std::array<std::shared_ptr<BoxPackNode>, 2> children;

        FTK_API bool isBranch() const;
        FTK_API bool isOccupied() const;
    };

    //! Pack boxes together.
    class FTK_API_TYPE BoxPack : public std::enable_shared_from_this<BoxPack>
    {
        FTK_NON_COPYABLE(BoxPack);

    private:
        BoxPack() = default;

    public:
        //! Create a new object.
        FTK_API static std::shared_ptr<BoxPack> create(const Size2I&, int border = 0);

        //! Get the root node.
        FTK_API const std::shared_ptr<BoxPackNode>& getRoot() const;

        //! Get the list of nodes.
        FTK_API std::vector<std::shared_ptr<BoxPackNode> > getNodes() const;

        //! Get the node with the given ID.
        FTK_API std::shared_ptr<BoxPackNode> getNode(BoxPackID);

        //! Insert a node.
        FTK_API std::shared_ptr<BoxPackNode> insert(const Size2I&);

    private:
        void _getNodes(
            const std::shared_ptr<BoxPackNode>&,
            std::vector<std::shared_ptr<BoxPackNode> >&) const;

        std::shared_ptr<BoxPackNode> _insert(
            std::shared_ptr<BoxPackNode>,
            const Size2I&);

        void _removeFromMap(const std::shared_ptr<BoxPackNode>&);

        int _border = 0;
        std::shared_ptr<BoxPackNode> _root;
        BoxPackID _id = 0;
        BoxPackTimestamp _timestamp = 0;
        std::map<BoxPackID, std::shared_ptr<BoxPackNode>> _idToNode;
    };

    ///@}
}
