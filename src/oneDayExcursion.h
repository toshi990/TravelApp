#ifndef SUCCESS_ONEDAYEXCURSION_H
#define SUCCESS_ONEDAYEXCURSION_H

#include "route.h"
#include "../exc/AExcursion.h"
#include "../interfaces/transfer.h"

/**
 * @brief Represents a single-day excursion.
 * * Simple excursion type that contains exactly one route.
 */
class OneDayExcursion : public Excursion {
public:
    /** @brief Default constructor. */
    OneDayExcursion() = default;

    /**
     * @brief Parameterized constructor.
     * @param id Unique identifier.
     * @param name Name of the excursion.
     * @param date Date of the trip.
     * @param p Price.
     * @param maxP Maximum capacity.
     */
    OneDayExcursion(std::string id, std::string name, std::chrono::year_month_day date, double p, size_t maxP)
    : Excursion(id, name, date, p, maxP) {}

    /** @return Const reference to the single day route. */
    const Route& getRoute() const { return route; }

    /** @return Reference to the single day route. */
    Route& getRoute() { return route; }

    /**
     * @brief Sets or replaces the route for this excursion.
     * @param points Vector of points defining the route.
     */
    void addRoute(const std::vector<PointData>& points) override {
        Route newRoute;
        for (const auto& p : points) {
            newRoute.addPoint(ExcursionPoint(p.locality, p.attraction, p.startTime, p.dur));
        }
        this->route = newRoute;
    }

    /**
     * @brief Serializes one-day excursion data to a stream.
     * * Format starts with the "ONE_DAY" header, followed by core attributes
     * * and the list of points in the route.
     * @param os Output stream.
     */
    void serialize(std::ostream& os) const noexcept override {
        os << "ONE_DAY\n";
        os << ID << "\n" << excursionName << "\n" << price << "\n" << maxParticipants << "\n";
        os << startDate.year() << " " << (unsigned)startDate.month() << " " << (unsigned)startDate.day() << "\n";
        os << "1" << "\n"; // Denotes a single route
        auto points = route.getPoints();
        os << points.size() << "\n";
        for (const auto& p : points) {
            os << p.getLocalityName() << "|" << p.getAttractionName() << "|"
               << p.getStartTime().time_since_epoch().count() << "|"
               << p.getDurationTime().count() << "\n";
        }
    }

    /** @brief Overridden as empty for one-day excursions (duration is not a separate field). */
    std::string getDurationFormat() const override { return ""; }

    /**
     * @brief Formats the route for display.
     * @return Vector containing route header and point strings.
     */
    std::vector<std::string> getFormattedRoutes() const noexcept override {
        std::vector<std::string> result;
        result.push_back("--- Single Day Route ---");
        for (const auto& point : route.getPoints()) {
            result.push_back(point.toString());
        }
        return result;
    }

    /**
     * @brief Validates if the excursion's route matches the provided list of points.
     * @param targetPoints Route points to check.
     * @return true if all points match in order.
     */
    bool containsRoute(const std::vector<PointData>& targetPoints) const override {
        auto points = route.toPointDataVector();
        if (points.size() != targetPoints.size()) return false;
        for (std::size_t i = 0; i < points.size(); i++) {
            if (points[i] != targetPoints[i]) return false;
        }
        return true;
    }

private:
    Route route; ///< The single route assigned to this excursion.
};

#endif //SUCCESS_ONEDAYEXCURSION_H