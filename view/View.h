#ifndef VIEW_H
#define VIEW_H
#include "IView.h"


/**
 * @brief Interface for the View component in the MVC pattern.
 * * This class defines the contract for user interaction. It handles all input
 * * operations and the visual representation of data provided by the Model.
 */
class View : public IView {
public:
    std::chrono::year_month_day getDate(const std::string& prompt) override {
        std::cout << prompt << " format YYYY-MM-DD: ";
        std::string dateStr;
        std::getline(std::cin, dateStr);
        int year, month, day;
        char dash1, dash2;
        std::istringstream iss(dateStr);
        if (iss >> year >> dash1 >> month >> dash2 >> day &&
            dash1 == '-' && dash2 == '-') {
            return std::chrono::year_month_day {
                std::chrono::year{year},
                std::chrono::month{static_cast<unsigned>(month)},
                std::chrono::day{static_cast<unsigned>(day)}
            };
        }
        else {
            throw std::invalid_argument("Invalid date format");
        }
    }

    std::chrono::sys_seconds parseTime(const std::chrono::year_month_day& baseDate) {
        while (true) {
            std::cout << "    Start time (HH:MM): ";
            std::string timeStr;
            std::getline(std::cin, timeStr);

            int hours, minutes;
            char colon;
            std::istringstream iss(timeStr);

            if (iss >> hours >> colon >> minutes && colon == ':' &&
                hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60) {
                auto sysDays = std::chrono::sys_days{baseDate};
                return sysDays + std::chrono::hours{hours} + std::chrono::minutes{minutes};
                }
            std::cout << "   Invalid time format, try HH:MM\n";
        }
    }

    PointData promptForPointData(std::chrono::year_month_day &date) {
        PointData p;
        std::cout << "    Locality: ";
        std::getline(std::cin, p.locality);
        std::cout << "    Attraction: ";
        std::getline(std::cin, p.attraction);
        std::cout << "    Duration (hours): ";
        double hrs;
        std::cin >> hrs;
        p.dur = std::chrono::duration<double>(std::chrono::hours(static_cast<int>(hrs)));
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        p.startTime = parseTime(date);
        return p;
    }

    const std::vector<PointData> promptForVectorPoints (std::chrono::year_month_day &date) override {
        std::vector<PointData> route;
        std::cout << "Amount of points: ";
        size_t pointsCount;
        std::cin >> pointsCount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (int i = 0; i < pointsCount; ++i) {
            route.push_back(promptForPointData(date));
        }
        return route;
    }

int displayMenu() override {
    std::cout << "\n=== Excursion Registry ===\n";
    std::cout << "1. Add new excursion\n";
    std::cout << "2. Cancel excursion for all dates\n";
    std::cout << "3. Make reservation\n";
    std::cout << "4. Cancel reservation\n";
    std::cout << "5. Cancel excursion \n";
    std::cout << "6. Show all excursions\n";
    std::cout << "7. Save to file\n";
    std::cout << "8. Load file\n";
    std::cout << "9. Find closest\n";
    std::cout << "10. Find closest using threads\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose the option between 0 and 9: ";
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 0;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::size_t promptForSize_t(const std::string& prompt) override {
        std::cout << prompt;
        std::size_t t;
        std::cin >> t;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return t;
}

std::tuple<bool, std::string, std::chrono::year_month_day, double, size_t,
    std::vector<std::vector<PointData>>, std::chrono::duration<double>> promptForExcursionDetails() override {

    std::cout << "Type (0 - OneDay, 1 - MultiDay): ";
    bool type;
    std::cin >> type;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Excursion name: ";
    std::string name;
    std::getline(std::cin, name);

    auto date = getDate("Date");

    std::cout << "Price: ";
    double price;
    std::cin >> price;

    std::cout << "Max participants: ";
    size_t maxParticipants;
    std::cin >> maxParticipants;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::chrono::duration<double> dur(0.0);
    std::size_t count = 1;
    if (type) {
        std::cout << "Total duration (hours): ";
        double hours;
        std::cin >> hours;
        dur = std::chrono::duration<double>(std::chrono::hours(static_cast<int>(hours)));

        std::cout << "Amount of routes: ";
        std::cin >> count;
    }

    std::vector<std::vector<PointData>> routes;
    for (std::size_t i = 0; i < count; i += 1) {
        std::cout << "  Day " << (i + 1) << " Amount of points in the route ";
        int pointsCount;
        std::cin >> pointsCount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::vector<PointData> dayPoints;
        for (int j = 0; j < pointsCount; j += 1) {
            std::cout << "  Point " << (j + 1) << ":\n";
            dayPoints.push_back(promptForPointData(date));
        }
        routes.push_back(dayPoints);
    }

    return {type, name, date, price, maxParticipants, routes, dur};
    }

    std::pair<std::string, std::chrono::year_month_day> promptForBookingDetails() override {
        std::cout << "ID: ";
        std::string name;
        std::getline(std::cin, name);

        auto date = getDate("Date");

        return std::make_pair(name, date);
    }

    void displayExcursion (const IExcursion* exc) override {
        if (exc == nullptr) {
            std::cout << "Can't read excursion\n";
            return;
        }
        auto date = exc->getStartDate();
        std::string dateStr = std::format("{:%Y-%m-%d}", date);

        std::cout << "EXCURSION: " << exc->getExcursionName() << "\n";
        std::cout << "    ID:           " << exc->getID() << "\n";
        std::cout << "    Date:         " << dateStr << "\n";
        std::cout << std::format("    Price:        {:.2f} $\n", exc->getPrice());
        std::cout << "    Participants: " << exc->getParticipantsCount()
              << " / " << exc->getMaxParticipants() << "\n";
        std::cout << exc->getDurationFormat() << "\n";


        auto routes = exc->getFormattedRoutes();
        if (!routes.empty()) {
            std::cout << "    Routes:\n";
            for (const auto& line : routes) {
                if (line.find("---") != std::string::npos) {
                    std::cout << "      " << line << "\n";
                } else {
                    std::cout << "        • " << line << "\n";
                }
            }
        }
        else {
            std::cout << "    Routes: No routes defined yet.\n";
        }

        std::cout << "----------------------------------------\n";
    }


    void displayExcursions(const std::vector<IExcursion*>& excursions) override {
        if (excursions.empty()) {
            showMessage("No excursions found");
            return;
        }

        std::cout << "\n========================================\n";
        std::cout << "       LIST OF ALL EXCURSIONS (" << excursions.size() << ")\n";
        std::cout << "========================================\n";

        for (size_t i = 0; i < excursions.size(); ++i) {
            if (excursions[i]) {
                auto date = excursions[i]->getStartDate();
                std::string dateStr = std::format("{:%Y-%m-%d}", date);

                std::cout << "\n[" << (i + 1) << "] EXCURSION: " << excursions[i]->getExcursionName() << "\n";
                std::cout << "    ID:           " << excursions[i]->getID() << "\n";
                std::cout << "    Date:         " << dateStr << "\n";
                std::cout << std::format("    Price:        {:.2f} $\n", excursions[i]->getPrice());
                std::cout << "    Participants: " << excursions[i]->getParticipantsCount()
                      << " / " << excursions[i]->getMaxParticipants() << "\n";
                std::cout << excursions[i]->getDurationFormat() << "\n";


                auto routes = excursions[i]->getFormattedRoutes();
                if (!routes.empty()) {
                    std::cout << "    Routes:\n";
                    for (const auto& line : routes) {
                        if (line.find("---") != std::string::npos) {
                            std::cout << "      " << line << "\n";
                        } else {
                            std::cout << "        • " << line << "\n";
                        }
                    }
                }
                else {
                    std::cout << "    Routes: No routes defined yet.\n";
                }

                std::cout << "----------------------------------------\n";
            }
        }
    }

    void showMessage(const std::string& message) override {
        std::cout << "\n" << message << "\n";
        std::cout << "======\n";
    }

    std::string promptForLine(const std::string& prompt) override {
        std::cout << prompt;
        std::string filename;
        std::getline(std::cin, filename);
        return filename;
    }

    void showTime (long long time) override {
        std::cout << "Time: " << time << std::endl;
    }
};

#endif