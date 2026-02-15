#ifndef TRAVEL_APP_IVIEW_H
#define TRAVEL_APP_IVIEW_H

#include <string>
#include <chrono>
#include <vector>
#include <tuple>
#include "../interfaces/transfer.h"
#include "../interfaces/IExcursionModel.h"

class IExcursion;

/**
 * @brief Interface for the View component in the MVC architecture.
 * * This interface defines the methods required for user interaction,
 * * including data input, menu navigation, and displaying excursion information.
 */
class IView {
public:
    virtual ~IView() = default;

    /**
     * @brief Displays the main application menu and captures user choice.
     * @return Selected menu option as an integer.
     */
    virtual int displayMenu() = 0;

    /**
     * @brief Prompts the user for all necessary details to create a new excursion.
     * @return A tuple containing:
     * - bool: True if multi-day, false if one-day.
     * - string: Excursion name.
     * - year_month_day: Start date.
     * - double: Price.
     * - size_t: Maximum participants.
     * - vector<vector<PointData>>: Routes for each day.
     * - duration<double>: Total duration.
     */
    virtual std::tuple<bool, std::string, std::chrono::year_month_day, double, size_t,
       std::vector<std::vector<PointData>>, std::chrono::duration<double>> promptForExcursionDetails() = 0;

    /**
     * @brief Displays a list of all available excursions.
     * @param excursions Vector of pointers to excursions to be listed.
     */
    virtual void displayExcursions(const std::vector<IExcursion*>& excursions) = 0;

    /**
     * @brief Shows a general text message or notification to the user.
     * @param message The string content to be displayed.
     */
    virtual void showMessage(const std::string& message) = 0;

    /**
     * @brief Prompts the user for a single line of string input.
     * @param prompt The message shown to the user before input.
     * @return The string entered by the user.
     */
    virtual std::string promptForLine(const std::string& prompt) = 0;

    /**
     * @brief Prompts the user to enter a date.
     * @param prompt The message shown to the user.
     * @return The entered date in year_month_day format.
     */
    virtual std::chrono::year_month_day getDate(const std::string& prompt) = 0;

    /**
     * @brief Displays detailed information about a specific excursion.
     * @param exc Pointer to the excursion object to be displayed.
     */
    virtual void displayExcursion(const IExcursion* exc) = 0;

    /**
     * @brief Prompts the user to define a sequence of points for a route.
     * @param date Reference to the date for which the route is being defined.
     * @return A vector of PointData structures representing the route.
     */
    virtual const std::vector<PointData> promptForVectorPoints(std::chrono::year_month_day &date) = 0;

    /**
     * @brief Prompts the user for a numeric size_t value.
     * @param prompt The message shown to the user.
     * @return The entered size_t value.
     */
    virtual std::size_t promptForSize_t(const std::string& prompt) = 0;

    /**
     * @brief Displays the execution time of an operation (e.g., search duration).
     * @param time Time in microseconds or milliseconds.
     */
    virtual void showTime(long long time) = 0;

    /**
     * @brief Prompts for the ID and date required for booking or canceling a seat.
     * @return A pair containing the excursion ID and the specific date.
     */
    virtual std::pair<std::string, std::chrono::year_month_day> promptForBookingDetails() = 0;
};

#endif //TRAVEL_APP_IVIEW_H