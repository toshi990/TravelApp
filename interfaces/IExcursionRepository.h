#ifndef IEXCURSIONREPOSITORY_H
#define IEXCURSIONREPOSITORY_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include "IExcursion.h"

/**
 * @brief Interface for the Excursion Repository.
 * * This interface defines the standard operations for storing,
 * * searching, and removing excursion objects.
 */
class IExcursionRepository {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IExcursionRepository() = default;

    /**
     * @brief Adds a new excursion to the repository.
     * @param excursion A unique pointer to the excursion object to be stored.
     * @return true if the excursion was added successfully, false otherwise.
     */
    virtual bool addExcursion(std::unique_ptr<IExcursion> excursion) = 0;

    /**
     * @brief Removes all versions of an excursion identified by a specific ID.
     * @param ID The unique identifier of the excursion to remove.
     */
    virtual void removeExcursion(const std::string& ID) = 0;

    /**
     * @brief Removes a specific version (instance) of an excursion.
     * @param ID The unique identifier of the excursion.
     * @param exPtr Pointer to the specific excursion instance to be deleted.
     */
    virtual void removeSpecificVersion(const std::string& ID, IExcursion* exPtr) = 0;

    /**
     * @brief Retrieves all excursions currently stored in the repository (read-only).
     * @return A vector of pointers to excursion objects.
     */
    [[nodiscard]] virtual std::vector<IExcursion*> getAllExcursions() const = 0;

    /**
     * @brief Retrieves all excursions currently stored in the repository.
     * @return A vector of pointers to excursion objects.
     */
    [[nodiscard]] virtual std::vector<IExcursion*> getAllExcursions() = 0;

    /**
     * @brief Returns the total number of excursion objects in the repository.
     * @return The size of the repository.
     */
    [[nodiscard]] virtual size_t getSize() const = 0;

    /**
     * @brief Finds a specific version of an excursion based on its ID and start date.
     * @param id The unique identifier of the excursion.
     * @param date The specific start date to match.
     * @return A pointer to the matching excursion, or nullptr if not found.
     */
    virtual IExcursion* findExcursionVersion(const std::string& id, std::chrono::year_month_day date) = 0;
};

#endif //TRAVEL_APP_IEXCURSIONREPOSITORY_H