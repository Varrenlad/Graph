#pragma once
#include "common.h"
class Timer {
private:
	std::chrono::time_point<std::chrono::system_clock> then;
public:
	Timer();
	void Update();
	size_t Elapsed();
	~Timer();
};

