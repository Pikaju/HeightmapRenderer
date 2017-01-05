#include "Timer.h"

#include <chrono>

std::chrono::high_resolution_clock::time_point g_programStartupTime = std::chrono::high_resolution_clock::now();

namespace util {

	Timer::Timer() : m_currentNanoseconds(getProgramNanoseconds())
	{
	}

	Timer::~Timer()
	{
	}

	unsigned long long Timer::getProgramNanoseconds()
	{
		return (unsigned long long)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - g_programStartupTime).count();
	}

}