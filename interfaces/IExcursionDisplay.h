#ifndef TRAVEL_APP_IEXCURSIONDISPLAY_H
#define TRAVEL_APP_IEXCURSIONDISPLAY_H

#include <string>
#include <chrono>

/**
 * @brief Interface for viewing basic excursion information.
 * * This interface provides read-only access to the essential details
 * * required to display an excursion in the user interface.
 */
class IExcursionDisplay {
public:
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~IExcursionDisplay() = default;

	/** @brief Returns the unique ID of the excursion for display. */
	[[nodiscard]] virtual std::string getID() const noexcept = 0;

	/** @brief Returns the name of the excursion. */
	[[nodiscard]] virtual std::string getExcursionName() const noexcept= 0;

	/** @brief Returns the date when the excursion takes place. */
	[[nodiscard]] virtual std::chrono::year_month_day getStartDate() const noexcept = 0;

	/** @brief Returns the price of a single ticket. */
	[[nodiscard]] virtual double getPrice() const noexcept = 0;

	/** @brief Returns the maximum number of people who can join. */
	[[nodiscard]] virtual size_t getMaxParticipants() const noexcept= 0;

	/** @brief Returns the current number of people who have booked. */
	[[nodiscard]] virtual size_t getParticipantsCount() const noexcept = 0;
};

#endif //TRAVEL_APP_IEXCURSIONDISPLAY_H