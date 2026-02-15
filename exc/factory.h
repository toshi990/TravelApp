#ifndef SUCCESS_FACTORY_H
#define SUCCESS_FACTORY_H
#include "../src/multiDayExcursion.h"
#include "../src/oneDayExcursion.h"
#include <memory>

/**
 * @brief Factory class to create different types of excursions.
 * * This class implements the Factory Pattern to instantiate either
 * OneDayExcursion or MultiDayExcursion objects based on a flag.
 */
class ExcursionFactory {
public:
	/**
	 * @brief Creates an instance of an excursion.
	 * * @param isMulti Boolean flag: true for MultiDayExcursion, false for OneDayExcursion.
	 * @param id The unique identifier for the excursion.
	 * @param name The display name of the excursion.
	 * @param date The scheduled start date.
	 * @param p The price of the excursion.
	 * @param maxP The maximum number of participants allowed.
	 * @param dur The duration of the excursion (used primarily for MultiDayExcursion).
	 * * @return std::unique_ptr<IExcursion> A unique pointer to the newly created excursion.
	 */
	static std::unique_ptr<IExcursion> create(bool isMulti, const std::string& id, const std::string& name,
		   std::chrono::year_month_day date, double p, std::size_t maxP, std::chrono::duration<double> dur) {
		if (isMulti) {
			return std::make_unique<MultiDayExcursion>(id, name, date, p, maxP, dur);
		}
		return std::make_unique<OneDayExcursion>(id, name, date, p, maxP);
	}
};

#endif //SUCCESS_FACTORY_H