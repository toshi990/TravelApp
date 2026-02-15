#ifndef SUCCESS_EXCURSIONPOINT_H
#define SUCCESS_EXCURSIONPOINT_H
#include <chrono>
#include <string>
#include <format>

/**
 * @brief Represents a specific stop or attraction within an excursion route.
 * * This class stores the geographic and temporal details for a single point of interest,
 * * including the location name, the specific attraction, start time, and visit duration.
 */
class ExcursionPoint {
public:
    /**
     * @brief Default constructor.
     */
    ExcursionPoint() = default;

    /**
     * @brief Parameterized constructor for initializing an excursion point.
     * @param locality The name of the city or locality.
     * @param attraction The name of the specific landmark or site.
     * @param start The scheduled start time of the visit.
     * @param duration The planned duration of time spent at this point.
     */
    ExcursionPoint(const std::string &locality, const std::string &attraction, std::chrono::sys_seconds start,
        std::chrono::duration<double> duration)
        : localityName(locality), attractionName(attraction), startTime(start), durationTime(duration) {}

    /** @brief Gets the name of the locality. @return Locality string. */
    std::string getLocalityName() const { return localityName; }

    /** @brief Gets the name of the attraction. @return Attraction string. */
    std::string getAttractionName() const { return attractionName; }

    /** @brief Gets the scheduled start time. @return sys_seconds timestamp. */
    std::chrono::sys_seconds getStartTime() const { return startTime; }

    /** @brief Gets the duration of the visit. @return duration object. */
    std::chrono::duration<double> getDurationTime() const { return durationTime; }

    /**
     * @brief Provides a formatted string representation of the excursion point.
     * * The output includes the start time, attraction name, duration in hours, and locality.
     * @return A formatted string suitable for display.
     */
    std::string toString() const {
        auto hrs = std::chrono::duration_cast<std::chrono::hours>(durationTime).count();
        return std::format("[{:%H:%M}] {} ({} hrs), Locality: {}", startTime, attractionName, hrs, localityName);
    }

private:
    std::string localityName;               ///< Name of the city/area.
    std::string attractionName;             ///< Name of the specific site.
    std::chrono::sys_seconds startTime;     ///< Scheduled arrival/start time.
    std::chrono::duration<double> durationTime; ///< How long the visit lasts.
};

#endif //SUCCESS_EXCURSIONPOINT_H