#ifndef SSBESB_PREPROCESSOR_H
#define SSBESB_PREPROCESSOR_H

#include "laic.h"
#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <array>
#include <string>

// Function to convert a vector to MyRange
template <typename T>
MyRange<T> From(const std::vector<T>& vec) {
    return MyRange<T>(vec);
}

// Function to convert a list to MyRange
template <typename T>
MyRange<T> From(const std::list<T>& list) {
    return MyRange<T>(list);
}

// Function to convert a set to MyRange
template <typename T>
MyRange<T> From(const std::set<T>& set) {
    return MyRange<T>(set);
}

// Function to convert a map to MyRange
template <typename K, typename T>
MyRange<T> From(const std::map<K, T>& map) {
    return MyRange<T>(map);
}

// Function to convert a deque to MyRange
template <typename T>
MyRange<T> From(const std::deque<T>& deque) {
    return MyRange<T>(deque);
}

// Function to convert an array to MyRange
template <typename T, size_t N>
MyRange<T> From(const T (&arr)[N]) {
    return MyRange<T>(arr);
}

// Function to preprocess code
void preprocessCode(const std::string& inputFile, const std::string& outputFile);

#endif // PREPROCESSOR_H
