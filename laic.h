#ifndef SSBESB_LAIC_H
#define SSBESB_LAIC_H

#include <iterator>
#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <deque>
#include <list>
#include <array>
#include <stdexcept>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <string>

// Base class for lazy operations
template <typename T>
class LazyOperation {
public:
    virtual ~LazyOperation() = default;
    virtual void Apply(std::vector<T>& data) const = 0;
};

// Lazy operation for filtering elements
template <typename T, typename Func>
class WhereOperation : public LazyOperation<T> {
public:
    WhereOperation(Func predicate) : predicate_(predicate) {}
    void Apply(std::vector<T>& data) const override {
        auto it = std::remove_if(data.begin(), data.end(), [&](const T& value) { return !predicate_(value); });
        data.erase(it, data.end());
    }
private:
    Func predicate_;
};

// Lazy operation for transforming elements
template <typename T, typename Func>
class SelectOperation : public LazyOperation<T> {
public:
    SelectOperation(Func selector) : selector_(selector) {}
    void Apply(std::vector<T>& data) const override {
        std::transform(data.begin(), data.end(), data.begin(), selector_);
    }
private:
    Func selector_;
};

// Lazy operation for custom operations
template <typename T, typename Func>
class CustomOperation : public LazyOperation<T> {
public:
    CustomOperation(Func operation) : operation_(operation) {}
    void Apply(std::vector<T>& data) const override {
        operation_(data);
    }
private:
    Func operation_;
};

// Class representing a range of elements with lazy operations
template <typename T>
class MyRange {
public:
    MyRange() = default;

     explicit MyRange(std::vector<T>&& data) : data_(std::move(data)) {}

    MyRange(const MyRange& other) = default;
    MyRange& operator=(const MyRange& other) = default;

    // Constructor that moves data from various containers
    explicit MyRange(std::vector<T>& data) : data_(std::move(data)) {}
    explicit MyRange(std::list<T>& data) : data_(std::move(data)) {}
    explicit MyRange(std::deque<T>& data) : data_(std::move(data)) {}
    explicit MyRange(std::set<T>& data) : data_(std::move(data)) {}

    // Constructor that takes an array
    template<size_t N>
    MyRange(const T(&arr)[N]) : data_(arr, arr + N) {}

    // Iterator methods
    typename std::vector<T>::iterator begin() { Evaluate(); return data_.begin(); }
    typename std::vector<T>::const_iterator begin() const { const_cast<MyRange*>(this)->Evaluate(); return data_.begin(); }
    typename std::vector<T>::iterator end() { Evaluate(); return data_.end(); }
    typename std::vector<T>::const_iterator end() const { const_cast<MyRange*>(this)->Evaluate(); return data_.end(); }

    // Lazy operations
    template <typename Predicate>
    MyRange<T> Where(Predicate predicate) const;

    template <typename Selector>
    auto Select(Selector selector) const -> MyRange<decltype(selector(std::declval<T>()))>;

    MyRange<T> Take(size_t count) const;
    MyRange<T> Skip(size_t count) const;
    MyRange<T> Concat(const MyRange& other) const;
    MyRange<T> Reverse() const;
    MyRange<T> Distinct() const;

    template <typename KeySelector>
    auto OrderBy(KeySelector keySelector) const -> MyRange<T>;

    template <typename KeySelector>
    auto GroupBy(KeySelector keySelector) const -> MyRange<std::pair<decltype(keySelector(std::declval<T>())), std::vector<T>>>;

    // Immediate operations
    bool All(std::function<bool(T)> predicate) const;
    bool Any(std::function<bool(T)> predicate) const;
    T Sum() const;
    double Average() const;
    T Min() const;
    T Max() const;
    size_t Count() const;
    bool Contains(const T& value) const;
    T ElementAt(size_t index) const;
    std::set<T> ToSet() const;
    std::vector<T> ToList() const;
    std::deque<T> ToDeque() const;
    std::vector<T> ToVector() const;

    // Special methods for std::string

    MyRange<T> AddPrefix(const std::string& prefix) const;
    MyRange<T> AddSuffix(const std::string& suffix) const;
    MyRange<T> ToUpperCase() const;
    MyRange<T> ToLowerCase() const;

private:
    mutable std::vector<T> data_;
    mutable std::vector<std::shared_ptr<LazyOperation<T>>> operations_;

    // Evaluate all pending operations
    void Evaluate() const {
        for (const auto& op : operations_) {
            op->Apply(data_);
        }
        operations_.clear();
    }
};

#include "laic_impl.h"

// Overloaded output operator for MyRange
template <typename T>
std::ostream& operator<<(std::ostream& os, const MyRange<T>& range) {
    os << "[";
    for (auto it = range.begin(); it != range.end(); ++it) {
        os << *it;
        if (std::next(it) != range.end()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

#endif // SSBESB_LAIC_H
