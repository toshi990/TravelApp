#ifndef SUCCESS_EXCURSIONMODEL_H
#define SUCCESS_EXCURSIONMODEL_H

#include "../interfaces/IExcursionModel.h"
#include "../interfaces/IExcursionRepository.h"
#include "../exc/factory.h"
#include "../interfaces/IFile.h"
#include "../interfaces/Iterator.h"
#include <future>
#include <thread>

using namespace std::chrono_literals;
using namespace std::chrono;

/**
 * @brief Implementation of the excursion management logic.
 * * Handles data processing, search algorithms, and file interactions.
 */
class ExcursionModel : public IExcursionModel {
private:
	std::unique_ptr<IExcursionRepository> repository; ///< Pointer to excursion storage.
	std::unique_ptr<IFile> fileSystem;                 ///< Pointer to file operations handler.

public:
	ExcursionModel(std::unique_ptr<IExcursionRepository> repo, std::unique_ptr<IFile> fs)
		: repository(std::move(repo)), fileSystem(std::move(fs)) {}

    /** @brief Converts raw point data into a Route object. */
	Route buildRoute(const std::vector<PointData>& data) {
		Route r;
		for (const auto& p : data) {
			r.addPoint(ExcursionPoint(p.locality, p.attraction,
                std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()), p.dur));
		}
		return r;
	}

    /** @brief Creates and adds a new excursion to the repository via Factory. */
	void announceExcursion(bool isMulti, const std::string& name, const std::chrono::year_month_day& date,
			double price, std::size_t maxP, const std::vector<std::vector<PointData>>& routes,
			std::chrono::duration<double> dur) override {

		std::string id = name.substr(0, 3) + "_ID";
		auto ex = ExcursionFactory::create(isMulti, id, name, date, price, maxP, dur);
		for (const auto& singleRouteData : routes) {
			ex->addRoute(singleRouteData);
		}
		repository->addExcursion(std::move(ex));
	}

    /** @brief Reserves a seat if available. */
	bool book(const std::string& ID, std::chrono::year_month_day date) override {
		IExcursion* ex = repository->findExcursionVersion(ID, date);
		if (ex && (ex->getMaxParticipants() - ex->getParticipantsCount()) > 0) {
			ex->modifyParticipantsCount(1);
			return true;
		}
		return false;
	}

    /** @brief Cancels a booking. */
	bool release(const std::string& ID, std::chrono::year_month_day date) override {
		IExcursion* ex = repository->findExcursionVersion(ID, date);
		if (ex) {
			ex->modifyParticipantsCount(-1);
			return true;
		}
		return false;
	}

    /** @brief Deletes a specific version (date) of an excursion. */
	void cancelSpecExcursion(const std::string& ID, std::chrono::year_month_day date) override {
		std::vector<IExcursion*> all = repository->getAllExcursions();
		IExcursion* toDelete = nullptr;

		for (auto ex : all) {
			if (ex->getID() == ID && ex->getStartDate() == date) {
				toDelete = ex;
				break;
			}
		}

		if (toDelete) {
			repository->removeSpecificVersion(ID, toDelete);
		}
	}

	void cancelExcursion(const std::string& ID) override {
		repository->removeExcursion(ID);
	}

	std::vector<IExcursion*> getAllExcursions() override {
		return repository->getAllExcursions();
	}

	void saveToFile(const std::string &fn) override {
		fileSystem->save(fn, repository.get());
	}

	void loadFile(const std::string &fn) override {
		fileSystem->load(fn, repository.get());
	}

    /** @brief Algorithm to find the closest matching excursion by date and route. */
	IExcursion* findClosestExcursion(std::chrono::year_month_day targetDate,
	    const std::vector<IExcursion*>& excursions, const std::vector<PointData>& targetRoute) override {
		if (excursions.empty()) return nullptr;
		IExcursion* bestFutureMatch = nullptr;
		IExcursion* absoluteLatestPast = nullptr;
		long long minDiff = std::numeric_limits<long long>::max();
		std::chrono::sys_days maxDate{std::chrono::days::min()};
		auto targetSysDays = std::chrono::sys_days{targetDate};

		ExcursionIterator it(excursions.begin());
		ExcursionIterator end(excursions.end());

		while (it != end) {
			IExcursion* ex = *it;
			if (ex->containsRoute(targetRoute)) {
				auto exSysDays = std::chrono::sys_days{ex->getStartDate()};
				if (exSysDays >= targetSysDays) {
					auto diff = (exSysDays - targetSysDays).count();
					if (diff < minDiff) {
						minDiff = diff;
						bestFutureMatch = ex;
					}
				} else if (exSysDays > maxDate) {
					maxDate = exSysDays;
					absoluteLatestPast = ex;
				}
			}
			++it;
		}
		return bestFutureMatch ? bestFutureMatch : absoluteLatestPast;
	}

    /** @brief Parallel version of the search algorithm using futures. */
	std::pair<IExcursion*, long long> findClosestExcursionParallel(std::chrono::year_month_day targetDate,
        const std::vector<IExcursion*>& allExcursions, const std::vector<PointData>& targetRoute,
        std::chrono::milliseconds timeout, std::size_t numThreads) override {

        auto startTime = std::chrono::steady_clock::now();
		if (allExcursions.empty()) return { nullptr, 0 };
		if (numThreads == 0) numThreads = 1;

		if (allExcursions.size() < numThreads) {
			IExcursion* res = findClosestExcursion(targetDate, allExcursions, targetRoute);
			auto endTime = std::chrono::steady_clock::now();
			return { res, std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() };
		}

		size_t chunkSize = allExcursions.size() / numThreads;
		std::vector<std::future<IExcursion*>> futures;

		for (unsigned int i = 0; i < numThreads; ++i) {
			auto startIt = allExcursions.begin() + i * chunkSize;
			auto endIt = (i == numThreads - 1) ? allExcursions.end() : (startIt + chunkSize);

			futures.push_back(std::async(std::launch::async, [this, targetDate, startIt, endIt, &targetRoute]() {
                std::vector<IExcursion*> subVector(startIt, endIt);
                return this->findClosestExcursion(targetDate, subVector, targetRoute);
			}));
		}

		std::vector<IExcursion*> candidates;
		auto stopTime = std::chrono::steady_clock::now() + timeout;

		for (auto& fut : futures) {
			auto now = std::chrono::steady_clock::now();
			if (now < stopTime && fut.wait_for(stopTime - now) == std::future_status::ready) {
				IExcursion* res = fut.get();
				if (res) candidates.push_back(res);
			}
		}

		IExcursion* finalResult = candidates.empty() ? nullptr : findClosestExcursion(targetDate, candidates, targetRoute);
		auto endTime = std::chrono::steady_clock::now();
		return { finalResult, std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() };
	}
};

#endif