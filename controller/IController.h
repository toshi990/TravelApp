#ifndef TRAVEL_APP_ICONTROLLER_H
#define TRAVEL_APP_ICONTROLLER_H

/**
 * @brief Interface for the Excursion Controller.
 * * Defines the main logic for handling user actions and managing excursions.
 */
class IController {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IController() = default;

    /**
     * @brief Starts the main application loop.
     */
    virtual void run() = 0;

    /**
     * @brief Adds a new excursion to the system.
     * * Takes user input to create a new excursion record.
     */
    virtual void handleAddExcursion() = 0;

    /**
     * @brief Removes all versions of a specific excursion.
     * * Deletes every record associated with the given ID.
     */
    virtual void handleCancelExcursion() = 0;

    /**
     * @brief Removes one specific date of an excursion.
     * * Deletes only the version that matches the ID and date.
     */
    virtual void handleCancelSpecExcursion() = 0;

    /**
     * @brief Processes a ticket purchase.
     * * Decreases the count of available seats for an excursion.
     */
    virtual void handleBooking() = 0;

    /**
     * @brief Processes a ticket return.
     * * Increases the count of available seats for an excursion.
     */
    virtual void handleReleasing() = 0;

    /**
     * @brief Finds the excursion with the nearest start date.
     */
    virtual void handleSearchClosest() = 0;

    /**
     * @brief Displays all excursions stored in the system.
     */
    virtual void handleShowAll() = 0;

    /**
     * @brief Saves current data to a file.
     */
    virtual void handleSaveToFile() = 0;

    /**
     * @brief Loads excursion data from a file.
     */
    virtual void handleLoadFile() = 0;

    /**
     * @brief Runs multi-threaded data processing tasks.
     */
    virtual void handleThreads() = 0;
};

#endif //TRAVEL_APP_ICONTROLLER_H