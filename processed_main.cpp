#include "laic.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include "laic.h"

int main() {
    // Example usage with different containers



//    std::vector<int> data = std::vector({1, 3, 2, 4, 5}); // Can be replaced with std::vector<int>, std::deque<int>, std::list<int>, etc.

    std::vector<int> data;
    for (int i = 0; i < 5; ++i) {
        data.push_back(i);
    }

    MyRange<int> rangeData(data);

    MyRange<int> skipResult;
    MyRange<int> concatResult;
    MyRange<int> reverseResult;
    MyRange<int> distinctResult;
    MyRange<int> orderByResult;
    MyRange<int> selectResult;
    MyRange<std::pair<int, std::vector<int>>> groupByResult;
    bool allGreaterThanZero;
    bool anyGreaterThanThree;
    bool containsThree;
    int elementAtTwo;
    std::set<int> toSetResult;
    std::vector<int> toListResult;
    std::deque<int> toDequeResult;
    std::vector<int> toVectorResult;
    int minResult;
    int maxResult;
    size_t countResult;
    float averageResult;

    // Special block for operations
{
    skipResult = rangeData.Skip(2);
    concatResult = rangeData.Concat(MyRange<int>({6, 7, 8}));
    reverseResult = rangeData.Reverse();
    orderByResult = rangeData.OrderBy([&](auto value){ return value; });
    groupByResult = rangeData.GroupBy([&](auto value){ return value % 2; });
    selectResult = rangeData.Where([&](auto value){ return value < 3; }).Select([&](auto value){ return value * 6; });
    distinctResult = rangeData.Concat(MyRange<int>({5, 5, 6})).Distinct().Reverse();
    allGreaterThanZero = rangeData.All([&](auto value){ return value > 0; });
    anyGreaterThanThree = rangeData.Any([&](auto value){ return value > 3; });
    containsThree = rangeData.Contains(3);
    elementAtTwo = rangeData.ElementAt(2);
    toSetResult = rangeData.ToSet();
    toListResult = rangeData.Where([&](auto value){ return value > 4; }).ToList();
    toDequeResult = rangeData.ToDeque();
    toVectorResult = rangeData.ToVector();
    minResult = rangeData.Where([&](auto value){ return value > 2; }).Select([&](auto value){ return value * 2; }).Min();
    maxResult = rangeData.Where([&](auto value){ return value > 2; }).Select([&](auto value){ return value * 2; }).Max();
    averageResult = rangeData.Where([&](auto value){ return value > 2; }).Select([&](auto value){ return value * 2; }).Average();
    countResult = rangeData.Where([&](auto value){ return value > 2; }).Select([&](auto value){ return value * 2; }).Count();
}

    // Output results
    std::cout << "Skip 2: " << skipResult << std::endl;
    std::cout << "Concat: " << concatResult << std::endl;
    std::cout << "Reverse: " << reverseResult << std::endl;
    std::cout << "Distinct: " << distinctResult << std::endl;
    std::cout << "OrderBy: " << orderByResult << std::endl;



    std::cout << "Select (x2): " << selectResult << std::endl;
    std::cout << "All > 0: " << std::boolalpha << allGreaterThanZero << std::endl;
    std::cout << "Any > 3: " << std::boolalpha << anyGreaterThanThree << std::endl;
    std::cout << "Contains 3: " << std::boolalpha << containsThree << std::endl;

    std::cout << "ToVector: " << MyRange<int>(toVectorResult) << std::endl;

    std::cout << "Min: " << minResult << std::endl;
    std::cout << "Max: " << maxResult << std::endl;
    std::cout << "Count: " << countResult << std::endl;
    std::cout << "Average: " << averageResult << std::endl;


    return 0;
}
