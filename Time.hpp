#pragma once

#include <chrono>

namespace Time {
	static std::chrono::steady_clock::time_point m_timeSinceLaunch = std::chrono::steady_clock::now();
	static std::chrono::milliseconds DeltaMiliseconds;
	static std::chrono::microseconds DeltaMicroseconds;

	inline unsigned int GetMiliSecondsSinceLaunch() {
		DeltaMiliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_timeSinceLaunch);
		return static_cast<unsigned int>(DeltaMiliseconds.count());
	}
	inline unsigned int GetMicroSecondsSinceLaunch() {
		DeltaMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_timeSinceLaunch);
		return static_cast<unsigned int>(DeltaMiliseconds.count());
	}
}