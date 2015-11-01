#pragma once
#ifndef PROFILE_H
#define PROFILE_H

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <ratio>
#include <utility>

#ifdef WIN32
#include <Windows.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
#else
#include <sys/resource>
#include <sys/time.h>
#endif

namespace time_clock {

	class ProfilerInstance {
	public:
		typedef std::chrono::steady_clock SteadyClock;
		typedef SteadyClock::time_point TimePoint;
		typedef std::chrono::duration<double, std::ratio<1, 1>> DurationTime;
		enum class MemoryUnit { KB_, MB_, GB_ };
	private:
		#define KB / 1024
		#define MB KB / 1024
		#define GB MB / 1024
	private:
		static DurationTime duringTime;
		static TimePoint startTime;
		static TimePoint finishTime;
	public:
		static void start();
		static void finish();
		static void dumpDuringTime(std::ostream& os = std::cout);
		static double second();
		static double millisecond();
		static size_t memory(MemoryUnit mu = MemoryUnit::KB_);
	};
}


#endif
