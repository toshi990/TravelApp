#ifndef TRAVEL_APP_EXCURSION_H
#define TRAVEL_APP_EXCURSION_H
#include "../interfaces/IExcursion.h"

/**
 * @brief Base class representing a general excursion.
 * * This class stores core information like ID, name, date, and participant limits.
 * * It implements the IExcursion interface.
 */
class Excursion : public IExcursion {
public:
    /**
     * @brief Default constructor.
     */
    Excursion() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Excursion() = default;

    /**
     * @brief Parameterized constructor to initialize an excursion.
     * @param id Unique identifier.
     * @param name Name of the tour.
     * @param date Scheduled start date.
     * @param p Price of one ticket.
     * @param maxP Maximum number of allowed participants.
     */
    Excursion(const std::string& id, const std::string& name, std::chrono::year_month_day date, double p, size_t maxP)
       : ID(id), excursionName(name), startDate(date), price(p), maxParticipants(maxP), participantsCount(0) {}

    /** @brief Returns the name of the excursion. */
   [[nodiscard]] std::string getExcursionName() const noexcept override;

    /** @brief Returns the scheduled start date. */
    [[nodiscard]] std::chrono::year_month_day getStartDate() const noexcept override;

    /** @brief Returns the price of the excursion. */
    [[nodiscard]] double getPrice() const noexcept override;

    /** @brief Returns the maximum capacity of participants. */
    [[nodiscard]] size_t getMaxParticipants() const noexcept override;

    /** @brief Returns the current number of registered participants. */
    [[nodiscard]] size_t getParticipantsCount() const noexcept override;

    /** @brief Returns the unique ID of the excursion. */
    [[nodiscard]] std::string getID() const noexcept override;

    /**
     * @brief Sets the current participant count directly.
     * @param count New number of participants.
     */
    void setParticipantsCount(std::size_t count) override;

    /**
     * @brief Updates the start date of the excursion.
     * @param nweDate New scheduled date.
     */
    void setStartDate(std::chrono::year_month_day nweDate) override;

    /**
     * @brief Adjusts the current participant count by a relative value.
     * @param change The value to add (positive) or subtract (negative).
     * @throw std::out_of_range If the result exceeds limits or falls below zero.
     */
    void modifyParticipantsCount(int change) override;

protected:
    std::string ID;                         ///< Unique identifier for the excursion.
    std::string excursionName;             ///< Display name of the tour.
    std::chrono::year_month_day startDate; ///< Date when the excursion starts.
    double price;                          ///< Cost per person.
    size_t maxParticipants;                ///< Limit for total participants.
    size_t participantsCount;              ///< Current number of participants.
};
#endif //TRAVEL_APP_EXCURSION_H