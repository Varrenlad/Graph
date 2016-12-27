#pragma once
#pragma warning(disable:4996) //fopen_s gives us nullptr somewhy

//#define RUN_TESTS

#define RANDOM_ENUM 100
#define TIMERAND
#define NIL -1
#define ASYNCIO

#define EXC_NO_DATA 1

#ifdef ASYNCIO
#include <functional>
#include <cstdio>
#include <thread>
#include <cassert>
#include <chrono>
#endif

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