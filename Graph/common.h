#pragma once

#define RANDOM_ENUM 100
#define TIMERAND

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>
#include <random>
#include <queue>

#ifdef TIMERAND
#include <ctime>
#else
#define SEED 0xDEADBEEF
#endif