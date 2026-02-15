#ifndef TRAVEL_APP_TR_H
#define TRAVEL_APP_TR_H
#include <string>
#include <chrono>

/**
 * @brief Data structure representing a single point of interest in an excursion route.
 * * Stores location details, the specific attraction, scheduled time, and visit duration.
 */
struct PointData {
	std::string locality;               ///< Name of the city or area.
	std::string attraction;             ///< Name of the specific site or landmark.
	std::chrono::duration<double> dur;  ///< Estimated time spent at the attraction.
	std::chrono::sys_seconds startTime; ///< Scheduled start time of the visit.

	/**
	 * @brief Compares two PointData objects for equality.
	 * * Checks locality, attraction, and duration.
	 * * For the start time, it compares only the time of day (minutes since start of day),
	 * * ignoring the specific calendar date.
	 * @param other The point to compare with.
	 * @return true if points are considered identical, false otherwise.
	 */
	bool operator==(const PointData& other) const {
		using namespace std::chrono;

		// Calculate minutes from the beginning of the day (1440 minutes in 24h)
		auto t1 = floor<minutes>(startTime).time_since_epoch().count() % 1440;
		auto t2 = floor<minutes>(other.startTime).time_since_epoch().count() % 1440;

		return locality == other.locality &&
			  attraction == other.attraction &&
			  dur == other.dur &&
			  t1 == t2;
	}
};

#endif