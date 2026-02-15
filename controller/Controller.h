#ifndef TRAVEL_APP_CONTROLLER_H
#define TRAVEL_APP_CONTROLLER_H
#include "IController.h"
#include "../interfaces/IExcursionModel.h"
#include "../view/IView.h"
#include <chrono>

/**
 * @brief Main implementation of the Excursion Controller.
 * * This class connects the View and the Model. It handles user requests,
 * updates the data through the Model, and shows results through the View.
 */
class Controller : public IController {
private:
    std::unique_ptr<IExcursionModel> model; ///< Pointer to the data logic (Model).
    std::unique_ptr<IView> view;           ///< Pointer to the user interface (View).

public:
    /**
     * @brief Constructor for the Controller.
     * @param model Unique pointer to the excursion model.
     * @param view Unique pointer to the view interface.
     */
    Controller(std::unique_ptr<IExcursionModel> model, std::unique_ptr<IView> view)
        : model(std::move(model)), view(std::move(view)) {}

    /**
     * @brief Asks the user for excursion details and adds it to the system.
     */
    void handleAddExcursion() override {
        auto [type, name, date, price, maxParticipants,
            routes, dur] = view->promptForExcursionDetails();
        model->announceExcursion(type, name, date, price, maxParticipants, routes, dur);
    }

    /**
     * @brief Runs the main menu loop.
     * * Keeps the application active until the user chooses to exit.
     */
    void run() override {
        bool running = true;
        while (running) {
            try {
                int choice = view->displayMenu();
                switch (choice) {
                    case 1: handleAddExcursion(); break;
                    case 2: handleCancelExcursion(); break;
                    case 3: handleBooking(); break;
                    case 4: handleReleasing();  break;
                    case 5: handleCancelSpecExcursion(); break;
                    case 6: handleShowAll(); break;
                    case 7: handleSaveToFile(); break;
                    case 8: handleLoadFile(); break;
                    case 9: handleSearchClosest(); break;
                    case 10: handleThreads(); break;
                    case 0:
                        running = false;
                        break;
                    default:
                        view->showMessage("Choose between 0 and 10: ");
                }
            }
            catch (const std::exception& e) {
                view->showMessage(e.what());
            }
        }
    }

    /**
     * @brief Handles a ticket booking request.
     * * Asks for an ID and date, then updates the seat count in the model.
     */
    void handleBooking() override {
        auto [code, date] = view->promptForBookingDetails();
        model->book(code, date);
    }

    /**
     * @brief Handles a ticket cancellation (release) request.
     * * Returns a seat to the available pool for a specific excursion.
     */
    void handleReleasing() override {
        auto [code, date] = view->promptForBookingDetails();
        model->release(code, date);
    }

    /**
     * @brief Gets all excursions from the model and displays them.
     */
	void handleShowAll() override {
		std::vector<IExcursion*> res = model->getAllExcursions();
        view->displayExcursions(res);
	}

    /**
     * @brief Asks for a filename and saves the excursion list to it.
     */
    void handleSaveToFile() override {
        std::string fn = view->promptForLine("File: ");
        model->saveToFile(fn);
    }

    /**
     * @brief Asks for a filename and loads data into the system.
     */
    void handleLoadFile() override {
        std::string fn = view->promptForLine("File: ");
        model->loadFile(fn);
    }

    /**
     * @brief Searches for the excursion most similar to the user's requirements.
     * * Compares dates and routes to find the best match.
     */
    void handleSearchClosest() override {
        std::chrono::year_month_day date = view->getDate("Date: ");
        std::vector<PointData> targetRoute = view->promptForVectorPoints(date);
        IExcursion* bestMatch = model->findClosestExcursion(date, model->getAllExcursions(), targetRoute);
        if (bestMatch != nullptr) view->displayExcursion(bestMatch);
        else view->showMessage("No matching excursions\n");
    }

    /**
     * @brief Searches for the closest excursion using multiple threads.
     * * Divides the work among several threads to increase speed.
     */
    void handleThreads() override {
        std::chrono::year_month_day date = view->getDate("Date: ");
        std::vector<PointData> targetRoute = view->promptForVectorPoints(date);
        std::size_t n = view->promptForSize_t("Threads: ");
        std::pair<IExcursion*, long long> pr =  model->findClosestExcursionParallel(date, model->getAllExcursions(), targetRoute,
            std::chrono::milliseconds(2000), n);
        IExcursion* bestMatch = pr.first;
        if (bestMatch != nullptr) view->displayExcursion(bestMatch);
        else view->showMessage("No matching excursions\n");
        view->showTime(pr.second);
    }

    /**
     * @brief Deletes a specific date of an excursion.
     */
    void handleCancelSpecExcursion() override {
        std::chrono::year_month_day date = view->getDate("Date: ");
        std::string ID = view->promptForLine ("ID: ");
        model->cancelSpecExcursion(ID, date);
    }

    /**
     * @brief Deletes all records associated with a specific excursion ID.
     */
    void handleCancelExcursion() override {
        std::string ID = view->promptForLine ("ID: ");
        model->cancelExcursion(ID);
    }
};

#endif //TRAVEL_APP_CONTROLLER_H