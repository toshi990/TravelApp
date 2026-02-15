#ifndef TRAVEL_APP_MULTIDAYEXCURSION_H
#define TRAVEL_APP_MULTIDAYEXCURSION_H

#include "route.h"
#include <iostream>
#include "../exc/AExcursion.h"
#include <stdexcept>
#include <format>

/**
 * @brief Represents an excursion that spans multiple days.
 * * This class manages a collection of routes (one for each day) and tracks
 * * the total cumulative duration of the trip.
 */
class MultiDayExcursion : public Excursion {
public:
    /** @brief Default constructor. */
    MultiDayExcursion() = default;

    /**
     * @brief Parameterized constructor.
     * @param id Unique identifier.
     * @param name Name of the excursion.
     * @param date Start date.
     * @param p Price.
     * @param maxP Maximum capacity.
     * @param dur Total planned duration.
     */
    MultiDayExcursion(std::string id, std::string name, std::chrono::year_month_day date, double p, size_t maxP, std::chrono::duration<double> dur)
    : Excursion(id, name, date, p, maxP), duration(dur) {}

    /** @return Total duration of the multi-day excursion. */
    std::chrono::duration<double> getDuration() const { return duration; }

    /** * @brief Formats the total duration for display.
     * @return String in format "Total Duration: X.X hours".
     */
    std::string getDurationFormat() const override {
        double hours = std::chrono::duration<double, std::ratio<3600>>(duration).count();
        return std::format("    Total Duration: {:.1f} hours", hours);
    }

    /**
     * @brief Accesses the route for a specific day.
     * @param idx Zero-based day index.
     * @return Const reference to the Route.
     * @throws std::out_of_range if index is invalid.
     */
    const Route& getRouteForDay(size_t idx) const {
        if (idx >= routes.size()) {
            throw std::out_of_range("Invalid index of the day");
        }
        return routes[idx];
    }

    /**
     * @brief Serializes multi-day excursion data to a stream.
     * * Format includes the "MULTI_DAY" header, basic info, total duration,
     * * and a nested list of points for each day's route.
     * @param os Output stream.
     */
    void serialize(std::ostream& os) const noexcept override {
        os << "MULTI_DAY\n";
        os << ID << "\n" << excursionName << "\n" << price << "\n" << maxParticipants << "\n";
        os << startDate.year() << " " << (unsigned)startDate.month() << " " << (unsigned)startDate.day() << "\n";
        os << duration.count() << "\n";

        os << routes.size() << "\n";
        for (const auto& route : routes) {
            auto& points = route.getPoints();
            os << points.size() << "\n";
            for (const auto& p : points) {
                os << p.getLocalityName() << "|" << p.getAttractionName() << "|"
                   << p.getStartTime().time_since_epoch().count() << "|"
                   << p.getDurationTime().count() << "\n";
            }
        }
    }

    /** @brief Non-const version of route access for a specific day. */
    Route& getRouteForDay(size_t idx) {
        if (idx >= routes.size()) {
            throw std::out_of_range("Invalid index of the day");
        }
        return routes[idx];
    }

    /** @return Const reference to all day routes. */
    const std::vector<Route>& getRoutes() const { return routes; }

    /** @return Reference to all day routes. */
    std::vector<Route>& getRoutes() { return routes; }

    /**
     * @brief Adds a new day route to the excursion.
     * @param points Vector of point data for the new day.
     */
    void addRoute(const std::vector<PointData>& points) override {
        Route newDayRoute;
        for (const auto& p : points) {
            newDayRoute.addPoint(ExcursionPoint(p.locality, p.attraction, p.startTime, p.dur));
        }
        this->routes.push_back(newDayRoute);
    }

    /**
     * @brief Returns a list of strings representing the schedule for all days.
     * @return Vector of formatted strings.
     */
    std::vector<std::string> getFormattedRoutes() const noexcept override {
        std::vector<std::string> result;
        for (size_t i = 0; i < routes.size(); ++i) {
            result.push_back("--- Day " + std::to_string(i + 1) + " ---");
            for (const auto& point : routes[i].getPoints()) {
                result.push_back(point.toString());
            }
        }
        return result;
    }

    /**
     * @brief Checks if any of the excursion days match the target route.
     * @param targetPoints The sequence of points to search for.
     * @return true if an exact match for one day is found.
     */
    bool containsRoute(const std::vector<PointData>& targetPoints) const override {
        for (auto &route : routes) {
            auto points = route.toPointDataVector();
            if (points.size() != targetPoints.size()) continue;

            bool dayMatches = true;
            for (std::size_t i = 0; i < points.size(); i++) {
                if (points[i] != targetPoints[i]) {
                    dayMatches = false;
                    break;
                }
            }
            if (dayMatches) return true;
        }
        return false;
    }

private:
    std::chrono::duration<double> duration; ///< Total cumulative duration.
    std::vector<Route> routes;              ///< List of routes, one per day.
};

#endif