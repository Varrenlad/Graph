#pragma once

#define RANDOM_ENUM 100
#define TIMERAND
#define NIL -1

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>
#include <random>
#include <queue>
#include <stack>
#include <list>
#include <iterator>
#ifdef TIMERAND
#include <ctime>
#else
#define SEED 0xDEADBEEF
#endif