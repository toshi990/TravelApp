#ifndef TRAVEL_APP_HASHTABLECHAIN_H
#define TRAVEL_APP_HASHTABLECHAIN_H

#include "excursionKey.h"
#include "../template/hashTable.h"
#include "../interfaces/IExcursionRepository.h"
#include <vector>
#include <memory>

/**
 * @brief Repository implementation using a Hash Table to store excursions.
 * * This class implements the IExcursionRepository interface, utilizing a custom
 * * HashTable to manage excursion objects. It supports versioning by storing
 * * multiple excursion instances under the same ID key.
 */
class HashTableRepository : public IExcursionRepository {
private:
    /** @brief Internal storage using a custom hash table template. */
    HashTable<ExcursionKey, std::unique_ptr<IExcursion>> table;

public:
    /**
     * @brief Constructs the repository with a specified hash table size.
     * @param size The initial number of buckets in the hash table.
     */
    explicit HashTableRepository(size_t size) : table(size) {}

    /**
     * @brief Adds a new excursion to the repository.
     * @param excursion Unique pointer to the excursion to be added.
     * @return true if the insertion was successful, false if the pointer is null or insertion failed.
     */
    bool addExcursion(std::unique_ptr<IExcursion> excursion) override {
        if (!excursion) return false;
        return table.insert(ExcursionKey(excursion->getID()), std::move(excursion)).second;
    }

    /**
     * @brief Finds a specific version of an excursion based on its ID and start date.
     * * Searches through the version chain associated with the given ID.
     * @param id The unique string identifier of the excursion.
     * @param date The specific date to match against the excursion's start date.
     * @return Pointer to the matching IExcursion, or nullptr if not found.
     */
    IExcursion* findExcursionVersion(const std::string& id, std::chrono::year_month_day date) override {
        auto result = table.findSpecificVersion(ExcursionKey(id), [&](const std::unique_ptr<IExcursion>& ex) {
            return ex && ex->getStartDate() == date;
        });
        return result ? result->get() : nullptr;
    }

    /**
     * @brief Removes a single specific excursion instance from the repository.
     * @param ID The unique identifier of the excursion.
     * @param exPtr Raw pointer to the specific version/instance to be removed.
     */
    void removeSpecificVersion(const std::string& ID, IExcursion* exPtr) override {
        table.eraseSpecificVersion(ExcursionKey(ID), exPtr);
    }

    /**
     * @brief Removes all versions associated with a given excursion ID.
     * @param ID The unique identifier to be completely removed from the table.
     */
    void removeExcursion(const std::string& ID) override {
        table.erase(ExcursionKey(ID));
    }

    /**
     * @brief Retrieves all excursion instances currently stored in the repository.
     * * Iterates through every bucket and every version node in the hash table to
     * * collect raw pointers to all excursions.
     * @return A vector of raw pointers to all IExcursion objects.
     */
    std::vector<IExcursion*> getAllExcursions() override {
        std::vector<IExcursion*> all;
        for (auto it = table.begin(); it != table.end(); ++it) {
            if (it->second) all.push_back(it->second.get());

            auto vCurr = it.curr->versionsHead;
            while (vCurr != nullptr) {
                if (vCurr->data && vCurr->data.get() != it->second.get()) {
                    all.push_back(vCurr->data.get());
                }
                vCurr = vCurr->nextV;
            }
        }
        return all;
    }

    /**
     * @brief Read-only version of getAllExcursions.
     * * Identical logic to the standard version but provides const-safe access.
     * @return A vector of raw pointers to all IExcursion objects.
     */
    std::vector<IExcursion*> getAllExcursions() const override {
        std::vector<IExcursion*> all;
        for (auto it = table.begin(); it != table.end(); ++it) {
            if (it->second) all.push_back(it->second.get());

            auto vCurr = it.curr->versionsHead;
            while (vCurr != nullptr) {
                IExcursion* rawPtr = vCurr->data ? vCurr->data.get() : nullptr;
                if (rawPtr && rawPtr != it->second.get()) {
                    all.push_back(rawPtr);
                }
                vCurr = vCurr->nextV;
            }
        }
        return all;
    }

    /**
     * @brief Gets the total number of unique keys (excursion groups) in the repository.
     * @return The count of main entries in the hash table.
     */
    size_t getSize() const override {
        return table.size();
    }
};

#endif // TRAVEL_APP_HASHTABLECHAIN_H