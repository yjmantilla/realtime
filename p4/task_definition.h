/*************************************************/
//  - Code by Prof. Gustavo Patino  -
//    - Real-Time Systems (IEO 2547027) -
//     - University of Antioquia (UdeA) -
//      - Medellin, Colombia 2023 -
/*************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <random>
#include <optional>

#ifndef LIB_HPP
#define LIB_HPP

std::optional<int> produce(int* c, int id);
std::optional<int> consume(int* c, int id);

#endif // LIB_HPP
