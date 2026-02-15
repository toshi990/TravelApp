#ifndef SUCCESS_ROUTE_H
#define SUCCESS_ROUTE_H

#include "excursionPoint.h"
#include <vector>
#include <stdexcept>

/**
 * @brief Represents a collection of excursion points forming a single trip route.
 * * This class manages a sequence of ExcursionPoint objects and provides utility
 * * methods for data conversion and access.
 */
class Route {
public:
    /**
     * @brief Default constructor.
     */
    Route() = default;

    /**
     * @brief Adds a new point to the end of the route.
     * @param newPoint The ExcursionPoint object to be added.
     */
    void addPoint(const ExcursionPoint &newPoint) {
        points.push_back(newPoint);
    }

    /**
     * @brief Retrieves a specific point by its index in the sequence.
     * @param index Zero-based index of the point.
     * @return Const reference to the ExcursionPoint.
     * @throws std::invalid_argument if the index is out of bounds.
     */
    const ExcursionPoint& getPoint(size_t index) const {
        if (index >= points.size()) {
            throw std::invalid_argument("Index is out of range");
        }
        return points[index];
    }

    /**
     * @brief Returns a read-only view of all points in the route.
     * @return Const reference to the underlying vector of points.
     */
    [[nodiscard]] const std::vector<ExcursionPoint>& getPoints() const {
        return points;
    }

    /**
     * @brief Returns a modifiable view of all points in the route.
     * @return Reference to the underlying vector of points.
     */
    [[nodiscard]] std::vector<ExcursionPoint>& getPoints() {
        return points;
    }

    /**
     * @brief Converts the route into a vector of PointData structures.
     * * Useful for transferring data between different layers of the application
     * * or for serialization purposes.
     * @return A vector containing PointData equivalents of each excursion point.
     */
    std::vector<PointData> toPointDataVector() const {
        std::vector<PointData> data;
        data.reserve(points.size());
        for (const auto& p : points) {
            data.push_back({
                p.getLocalityName(),
                p.getAttractionName(),
                p.getDurationTime(),
                p.getStartTime()
            });
        }
        return data;
    }

private:
    std::vector<ExcursionPoint> points; ///< Internal storage for the sequence of points.
};

#endif //SUCCESS_ROUTE_H