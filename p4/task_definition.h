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

#ifndef LIB_HPP
#define LIB_HPP

void produce(int* c, int id);
void consume(int* c, int id);

#endif // LIB_HPP
