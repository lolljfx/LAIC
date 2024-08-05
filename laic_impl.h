#ifndef SSBESB_LAIC_IMPL_H
#define SSBESB_LAIC_IMPL_H

#include "laic.h"

// Implementation of Where operation
template <typename T>
template <typename Predicate>
MyRange<T> MyRange<T>::Where(Predicate predicate) const {
    MyRange<T> result = *this;
    result.operations_.push_back(std::make_shared<WhereOperation<T, Predicate>>(predicate));
    return result;
}

// Implementation of Select operation
template <typename T>
template <typename Selector>
auto MyRange<T>::Select(Selector selector) const -> MyRange<decltype(selector(std::declval<T>()))> {
    MyRange<decltype(selector(std::declval<T>()))> result(ToVector());
    result.operations_.push_back(std::make_shared<SelectOperation<T, Selector>>(selector));
    return result;
}

// Implementation of Take operation
template <typename T>
MyRange<T> MyRange<T>::Take(size_t count) const {
    MyRange<T> result = *this;
    result.operations_.push_back(std::make_shared<CustomOperation<T, std::function<void(std::vector<T>&)>>>([count](std::vector<T>& data) {
        if (count < data.size()) {
            data.resize(count);
        }
    }));
    return result;
}

// Implementation of Skip operation
template <typename T>
MyRange<T> MyRange<T>::Skip(size_t count) const {
    MyRange<T> result = *this;
    result.operations_.push_back(std::make_shared<CustomOperation<T, std::function<void(std::vector<T>&)>>>([count](std::vector<T>& data) {
        if (count < data.size()) {
            data.erase(data.begin(), data.begin() + count);
        } else {
            data.clear();
        }
    }));
    return result;
}

// Implementation of Concat operation
template <typename T>
MyRange<T> MyRange<T>::Concat(const MyRange& other) const {
    MyRange<T> result = *this;
    result.operations_.push_back(std::make_shared<CustomOperation<T, std::function<void(std::vector<T>&)>>>([other](std::vector<T>& data) {
        auto otherData = other.ToVector();
        data.insert(data.end(), otherData.begin(), otherData.end());
    }));
    return result;
}

// Implementation of Reverse operation
template <typename T>
MyRange<T> MyRange<T>::Reverse() const {
    MyRange<T> result = *this;
    result.operations_.push_back(std::make_shared<CustomOperation<T, std::function<void(std::vector<T>&)>>>([](std::vector<T>& data) {
        std::reverse(data.begin(), data.end());
    }));
    return result;
}

// Implementation of Distinct operation
template <typename T>
MyRange<T> MyRange<T>::Distinct() const {
    MyRange<T> result = *this;
    result.operations_.push_back(std::make_shared<CustomOperation<T, std::function<void(std::vector<T>&)>>>([](std::vector<T>& data) {
        std::set<T> unique_elements(data.begin(), data.end());
        data.assign(unique_elements.begin(), unique_elements.end());
    }));
    return result;
}

// Implementation of OrderBy operation
template <typename T>
template <typename KeySelector>
auto MyRange<T>::OrderBy(KeySelector keySelector) const -> MyRange<T> {
    MyRange<T> result = *this;
    result.operations_.push_back(std::make_shared<CustomOperation<T, std::function<void(std::vector<T>&)>>>([keySelector](std::vector<T>& data) {
        std::sort(data.begin(), data.end(), [&](const T& a, const T& b) {
            return keySelector(a) < keySelector(b);
        });
    }));
    return result;
}

// Implementation of GroupBy operation
template <typename T>
template <typename KeySelector>
auto MyRange<T>::GroupBy(KeySelector keySelector) const -> MyRange<std::pair<decltype(keySelector(std::declval<T>())), std::vector<T>>> {
    using KeyType = decltype(keySelector(std::declval<T>()));
    std::map<KeyType, std::vector<T>> groups;
    for (const auto& item : data_) {
        groups[keySelector(item)].push_back(item);
    }
    std::vector<std::pair<KeyType, std::vector<T>>> result(groups.begin(), groups.end());
    return MyRange<std::pair<KeyType, std::vector<T>>>(result);
}


// Implementation of All operation
template <typename T>
bool MyRange<T>::All(std::function<bool(T)> predicate) const {
    Evaluate();
    return std::all_of(data_.begin(), data_.end(), predicate);
}

// Implementation of Any operation
template <typename T>
bool MyRange<T>::Any(std::function<bool(T)> predicate) const {
    Evaluate();
    return std::any_of(data_.begin(), data_.end(), predicate);
}

// Implementation of Sum operation
template <typename T>
T MyRange<T>::Sum() const {
    Evaluate();
    return std::accumulate(data_.begin(), data_.end(), T(0));
}

// Implementation of Average operation
template <typename T>
double MyRange<T>::Average() const {
    Evaluate();
    if (data_.empty()) return 0;
    return static_cast<double>(Sum()) / data_.size();
}

// Implementation of Min operation
template <typename T>
T MyRange<T>::Min() const {
    Evaluate();
    if (data_.empty()) throw std::logic_error("Empty range");
    return *std::min_element(data_.begin(), data_.end());
}

// Implementation of Max operation
template <typename T>
T MyRange<T>::Max() const {
    Evaluate();
    if (data_.empty()) throw std::logic_error("Empty range");
    return *std::max_element(data_.begin(), data_.end());
}

// Implementation of Count operation
template <typename T>
size_t MyRange<T>::Count() const {
    Evaluate();
    return data_.size();
}

// Implementation of Contains operation
template <typename T>
bool MyRange<T>::Contains(const T& value) const {
    Evaluate();
    return std::find(data_.begin(), data_.end(), value) != data_.end();
}

// Implementation of ElementAt operation
template <typename T>
T MyRange<T>::ElementAt(size_t index) const {
    Evaluate();
    if (index >= data_.size()) throw std::out_of_range("Index out of range");
    return data_[index];
}

// Implementation of ToSet operation
template <typename T>
std::set<T> MyRange<T>::ToSet() const {
    Evaluate();
    return std::set<T>(data_.begin(), data_.end());
}

// Implementation of ToList operation
template <typename T>
std::vector<T> MyRange<T>::ToList() const {
    Evaluate();
    return data_;
}

// Implementation of ToDeque operation
template <typename T>
std::deque<T> MyRange<T>::ToDeque() const {
    Evaluate();
    return std::deque<T>(data_.begin(), data_.end());
}

// Implementation of ToVector operation
template <typename T>
std::vector<T> MyRange<T>::ToVector() const {
    Evaluate();
    return data_;
}


// Implementation of AddPrefix operation for std::string
template <typename T>
MyRange<T> MyRange<T>::AddPrefix(const std::string& prefix) const {
    static_assert(std::is_same<T, std::string>::value, "AddPrefix is only supported for std::string type");
    return Select([prefix](const std::string& value) { return prefix + value; });
}

// Implementation of AddSuffix operation for std::string
template <typename T>
MyRange<T> MyRange<T>::AddSuffix(const std::string& suffix) const {
    static_assert(std::is_same<T, std::string>::value, "AddSuffix is only supported for std::string type");
    return Select([suffix](const std::string& value) { return value + suffix; });
}

// Implementation of ToUpperCase operation for std::string
template <typename T>
MyRange<T> MyRange<T>::ToUpperCase() const {
    static_assert(std::is_same<T, std::string>::value, "ToUpperCase is only supported for std::string type");
    return Select([](const std::string& value) {
        std::string result = value;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    });
}

// Implementation of ToLowerCase operation for std::string
template <typename T>
MyRange<T> MyRange<T>::ToLowerCase() const {
    static_assert(std::is_same<T, std::string>::value, "ToLowerCase is only supported for std::string type");
    return Select([](const std::string& value) {
        std::string result = value;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    });
}



#endif // SSBESB_LAIC_IMPL_H
