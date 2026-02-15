#ifndef IEXCURSION_H
#define IEXCURSION_H
#include <string>
#include <chrono>
#include <vector>
#include <ostream>
#include "IExcursionDisplay.h"
#include "transfer.h"

/**
 * @brief Base interface for all excursion types.
 * * This interface defines the core properties and behaviors of an excursion,
 * * such as managing participants, dates, and routes.
 * * It inherits from IExcursionDisplay for visualization purposes.
 */
class IExcursion : public IExcursionDisplay {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IExcursion() = default;

    /** @brief Returns the unique identifier of the excursion. */
    [[nodiscard]] virtual std::string getID() const noexcept = 0;

    /** @brief Returns the name of the excursion. */
	[[nodiscard]] virtual std::string getExcursionName() const noexcept = 0;

    /** @brief Returns the scheduled start date. */
	[[nodiscard]] virtual std::chrono::year_month_day getStartDate() const noexcept = 0;

    /** @brief Returns the cost of the excursion. */
	[[nodiscard]] virtual double getPrice() const noexcept = 0;

    /** @brief Returns the maximum number of people allowed. */
	[[nodiscard]] virtual size_t getMaxParticipants() const noexcept= 0;

    /** @brief Returns the current number of registered participants. */
	[[nodiscard]] virtual size_t getParticipantsCount() const noexcept = 0;

    /** * @brief Sets the participant count to a specific value.
     * @param count The new number of participants.
     */
    virtual void setParticipantsCount(size_t count) = 0;

    /** * @brief Updates the excursion's start date.
     * @param newDate The new scheduled date.
     */
    virtual void setStartDate(std::chrono::year_month_day newDate) = 0;

    /** * @brief Adds or subtracts from the current participant count.
     * @param change Positive value to add, negative to subtract.
     */
    virtual void modifyParticipantsCount(int change) = 0;

    /** * @brief Adds a new route consisting of multiple points to the excursion.
     * @param points A vector of PointData structures.
     */
	virtual void addRoute(const std::vector<PointData>& points) = 0;

    /** @brief Returns a list of routes formatted as strings for display. */
	virtual std::vector<std::string> getFormattedRoutes() const noexcept = 0;

    /** @brief Returns the duration of the excursion in a readable string format. */
	virtual std::string getDurationFormat() const = 0;

    /** * @brief Saves the excursion data to an output stream.
     * @param os The output stream (e.g., a file stream).
     */
	virtual void serialize(std::ostream& os) const noexcept = 0;

    /** * @brief Checks if the excursion includes a specific sequence of points.
     * @param targetPoints The route to search for.
     * @return true if the route is found, false otherwise.
     */
	virtual bool containsRoute (const std::vector<PointData>& targetPoints) const = 0;
};

#endif