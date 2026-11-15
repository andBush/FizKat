#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

template<typename T>
class Registry {
    std::vector<T> items_;

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    void add(T item) {
        items_.push_back(std::move(item));
    }

    std::size_t size() const noexcept {
        return items_.size();
    }

    iterator begin() noexcept { return items_.begin(); }
    iterator end() noexcept { return items_.end(); }
    const_iterator begin() const noexcept { return items_.begin(); }
    const_iterator end() const noexcept { return items_.end(); }

    template<typename Predicate>
    iterator findIf(Predicate predicate) {
        return std::find_if(begin(), end(), std::move(predicate));
    }

    void erase(iterator position) {
        items_.erase(position);
    }
};
