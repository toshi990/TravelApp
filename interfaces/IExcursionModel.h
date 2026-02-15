#ifndef TRAVEL_APP_IEXCURSIONMODEL_H
#define TRAVEL_APP_IEXCURSIONMODEL_H
#include "IExcursion.h"
#include <string>
#include <chrono>
#include <vector>
#include "transfer.h"

/**
 * @brief Interface for the Excursion Model logic.
 * * Defines how the application manages excursions, including creation,
 * * booking, file storage, and search algorithms.
 */
class IExcursionModel {
public:
    virtual ~IExcursionModel() = default;

    /**
     * @brief Creates and registers a new excursion in the system.
     * @param isMulti True if the excursion lasts multiple days.
     * @param excursionName The display name.
     * @param date The scheduled start date.
     * @param price Cost per participant.
     * @param maxParticipants Total seat capacity.
     * @param routes Nested vector of points defining available paths.
     * @param dur Total duration of the trip.
     */
	virtual void announceExcursion(bool isMulti, const std::string& excursionName, const std::chrono::year_month_day& date,
        double price, std::size_t maxParticipants, const std::vector<std::vector<PointData>>& routes, std::chrono::duration<double> dur) = 0;

    /** @brief Deletes all versions of an excursion by its ID. */
	virtual void cancelExcursion(const std::string& ID) = 0;

    /** @brief Reserves a seat for a specific ID and date. @return True if successful. */
	virtual bool book(const std::string& ID, std::chrono::year_month_day date) = 0;

    /** @brief Returns a seat to the pool. @return True if successful. */
	virtual bool release(const std::string& ID, std::chrono::year_month_day date) = 0;

    /** @brief Returns a list of all excursions currently in the system. */
	virtual std::vector<IExcursion*> getAllExcursions() = 0;

    /** @brief Removes only one specific date/version of an excursion. */
	virtual void cancelSpecExcursion(const std::string& ID, std::chrono::year_month_day date) = 0;

    /** @brief Saves all data to a specified filename. */
	virtual void saveToFile(const std::string &fn) = 0;

    /** @brief Loads data from a specified filename into the repository. */
	virtual void loadFile(const std::string &fn) = 0;

    /** * @brief Finds the excursion closest to the target date that contains a specific route.
     * * Priority is given to future excursions.
     */
	virtual IExcursion* findClosestExcursion(std::chrono::year_month_day targetDate,
		const std::vector<IExcursion*>& excursions, const std::vector<PointData>& targetRoute) = 0;

    /** * @brief Searches for the closest excursion using multiple threads for performance.
     * @param timeout Maximum time allowed for the parallel search.
     * @param numThreads Number of threads to use.
     * @return A pair containing the best match and the time taken in microseconds.
     */
	virtual std::pair<IExcursion*, long long> findClosestExcursionParallel(std::chrono::year_month_day targetDate,const std::vector<IExcursion*>& allExcursions,
		const std::vector<PointData>& targetRoute, std::chrono::milliseconds timeout, std::size_t numThreads) = 0;
};

#endif //TRAVEL_APP_IEXCURSIONMODEL_H