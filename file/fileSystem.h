#ifndef SUCCESS_FILESYSTEM_H
#define SUCCESS_FILESYSTEM_H
#include "../interfaces/IFile.h"
#include <fstream>
#include <sstream>
#include <vector>

/**
 * @brief Implementation of the IFile interface for data persistence.
 * * This class handles saving and loading excursion data to and from text files.
 * * It uses a custom serialization format to store excursion details and routes.
 */
class File : public IFile {
public:
    /**
     * @brief Saves all excursions from a repository to a file.
     * @param filename The path to the destination file.
     * @param repository Pointer to the repository containing the data.
     * @throw std::runtime_error If the file cannot be opened for writing.
     */
    void save(const std::string& filename, IExcursionRepository* repository) override {
        if (!repository) return;

        std::ofstream ofs(filename);
        if (!ofs.is_open()) {
            throw std::runtime_error("Could not open file the file for writing");
        }
        auto excursions = repository->getAllExcursions();
        ofs << excursions.size() << "\n";
        for (auto* ex : excursions) {
            ex->serialize(ofs);
        }
        ofs.close();
    }

    /**
     * @brief Parses a single line of text into a PointData object.
     * * The line should use the pipe ('|') character as a separator.
     * @param line A string containing locality, attraction, time, and duration.
     * @return PointData A structured object containing the parsed point information.
     */
    PointData parsePointLine(const std::string& line) {
        PointData pd;
        std::vector<std::string> parts;
        std::string part;
        std::stringstream ss(line);

        while (std::getline(ss, part, '|')) {
            parts.push_back(part);
        }

        if (parts.size() == 4) {
            pd.locality = parts[0];
            pd.attraction = parts[1];

            long long timeRaw = std::stoll(parts[2]);
            pd.startTime = std::chrono::sys_seconds{std::chrono::seconds{timeRaw}};

            double durRaw = std::stod(parts[3]);
            pd.dur = std::chrono::duration<double>(durRaw);
        }
        return pd;
    }

    /**
     * @brief Loads excursion data from a file into a repository.
     * * Reads the file line by line, recreates objects via ExcursionFactory,
     * * and populates the repository.
     * @param filename The path to the source file.
     * @param repository Pointer to the repository where data will be stored.
     * @throw std::runtime_error If the file cannot be opened for reading.
     */
    void load(const std::string& filename, IExcursionRepository* repository) override {
        if (!repository) return;

        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            throw std::runtime_error("Can't open the file for reading");
        }

        size_t count;
        if (!(ifs >> count)) return;

        for (size_t i = 0; i < count; ++i) {
            ifs >> std::ws;
            std::string type;
            std::getline(ifs, type);

            std::string id, name;
            std::getline(ifs, id);
            std::getline(ifs, name);

            double price;
            size_t maxP;
            int y, m, d;
            if (!(ifs >> price >> maxP >> y >> m >> d)) break;

            std::chrono::year_month_day date{std::chrono::year(y), std::chrono::month(m), std::chrono::day(d)};
            std::chrono::duration<double> duration{0};
            bool isMulti = (type == "MULTI_DAY");

            if (isMulti) {
                double durRaw;
                ifs >> durRaw;
                duration = std::chrono::duration<double>(durRaw);
            }

            auto ex = ExcursionFactory::create(isMulti, id, name, date, price, maxP, duration);
            size_t routesCount;
            ifs >> routesCount;
            ifs >> std::ws;

            for (size_t r = 0; r < routesCount; ++r) {
                size_t pointsCount;
                ifs >> pointsCount;
                ifs >> std::ws;

                std::vector<PointData> points;
                for (size_t p = 0; p < pointsCount; ++p) {
                    std::string line;
                    if (std::getline(ifs, line) && !line.empty()) {
                        points.push_back(parsePointLine(line));
                    }
                }
                ex->addRoute(points);
            }
            repository->addExcursion(std::move(ex));
        }
        ifs.close();
    }
};

#endif //SUCCESS_FILESYSTEM_H