#include <iostream>
#include <vector>
#include <string>


int main()
{
    std::string str1="Hello";
    std::string str2="World";

    if (str1 == str2)
    {
        std::cout<<"str1 equal str2"<<std::endl;
        return 0;
    }

    std::cout<<"str1 not equal str2"<<std::endl;
    return -1;
}
