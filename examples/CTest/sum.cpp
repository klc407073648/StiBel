#include <vector>
#include "sum.h"
int sum(const std::vector<int> num)
{
    auto sum = 0;
    for (auto i : num)
    {
        sum += i;
    }
    return sum;
}