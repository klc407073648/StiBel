#include <iostream>
#include <vector>
#include <string>
#include "sum.h"

int main()
{
    std::vector<int> nums = {1, 2, 3, 4, 5};
    if (sum(nums) == 15)
    {
        return 0;
    }
    return 1;
}
