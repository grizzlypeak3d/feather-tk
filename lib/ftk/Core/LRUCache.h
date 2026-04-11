// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <cstdint>
#include <list>
#include <unordered_map>
#include <vector>

namespace ftk
{
    //! Least recently used (LRU) cache.
    //!
    //! Uses a linked-hashmap structure: a std::list maintains LRU order and
    //! an unordered_map provides O(1) lookup.
    template<typename T, typename U>
    class LRUCache
    {
    public:
        //! \name Size
        ///@{

        size_t getMax() const;
        size_t getSize() const;
        size_t getCount() const;
        float getPercentage() const;

        void setMax(size_t);

        ///@}

        //! \name Contents
        ///@{

        bool contains(const T& key) const;
        bool get(const T& key, U& value);
        bool touch(const T& key);

        void add(const T& key, const U& value, size_t size = 1);
        void remove(const T& key);
        void clear();

        std::vector<T> getKeys() const;
        std::vector<U> getValues() const;

        ///@}

    private:
        void _evict();

        size_t _max  = 10000;
        size_t _size = 0; // running total of item sizes

        // LRU order: front = most-recently-used, back = least-recently-used.
        // Each node stores: key, value, item-size.
        using ListNode = std::tuple<T, U, size_t>;
        std::list<ListNode> _list;

        // Maps key -> iterator into _list for O(1) lookup and splice.
        using Map = std::unordered_map<T, typename std::list<ListNode>::iterator>;
        Map _map;
    };
}

#include <ftk/Core/LRUCacheInline.h>
