#pragma once

namespace ft {}
using namespace ft;

#include <vector>
#include <iostream>
#include <stack>
#include <iterator>
#include "stack/stack.hpp"
#include "vector/vector.hpp"
#include "map/map.hpp"
#include <map>

template <typename T>
void print_vec(vector <T> &v)
{
    for (size_t i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";
    std::cout << "capacity:" << v.capacity();
    std::cout << " size:" << v.size() << std::endl;
    std::cout << std::endl;
}