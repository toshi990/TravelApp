#include <iostream>
#include <memory>
#include "controller/Controller.h"
#include "view/View.h"
#include "model/ExcursionModel.h"
#include "repo/excursionRepository.h"
#include "file/fileSystem.h"
#include <thread>

void runSingleFileBenchmark(ExcursionModel& model, IExcursionRepository* repo, File* loader, const std::string& fname) {
	std::vector<int> threadConfigs = {1, 2, 4, 8, 10};
	const int iterations = 10;

	try {
		loader->load(fname, repo);
	} catch (const std::exception& e) {
		std::cout << "Ошибка при открытии файла: " << e.what() << std::endl;
		return;
	}

	auto allExcursions = repo->getAllExcursions();
	if (allExcursions.empty()) {
		std::cout << "Файл пуст или данные не загружены." << std::endl;
		return;
	}

	IExcursion* target = allExcursions[0];
	std::vector<PointData> targetRoute;

	auto oneDay = dynamic_cast<OneDayExcursion*>(target);
	if (oneDay) {
		targetRoute = oneDay->getRoute().toPointDataVector();
	} else {
		auto multiDay = dynamic_cast<MultiDayExcursion*>(target);
		if (multiDay) {
			targetRoute = multiDay->getRoutes()[0].toPointDataVector();
		}
	}
	if (targetRoute.empty()) {
		std::cout << "Ошибка: Маршрут пуст!" << std::endl;
		return;
	}

	auto targetDate = target->getStartDate();

	std::cout << "\nРезультаты для файла: " << fname << " (" << allExcursions.size() << " записей)" << std::endl;
	std::cout << "Threads | Avg Time (us)" << std::endl;
	std::cout << "-----------------------" << std::endl;

	for (int numThreads : threadConfigs) {
		long long totalTime = 0;

		for (int i = 0; i < iterations; ++i) {
			auto result = model.findClosestExcursionParallel(
				targetDate,
				allExcursions,
				targetRoute,
				std::chrono::milliseconds(5000),
				numThreads
			);
			totalTime += result.second;
		}

		long long avgTime = totalTime / iterations;
		std::cout << std::setw(7) << numThreads << " | " << avgTime << " us" << std::endl;
	}
}

int main() {
	try {
		std::cout <<std::thread::hardware_concurrency();
		auto fileSystem = std::make_unique<File>();
		auto repository = std::make_unique<HashTableRepository>(100);
		auto repoPtr = repository.get();
		auto fsPtr = fileSystem.get();

		auto model = std::make_unique<ExcursionModel>(
			std::move(repository),
			std::move(fileSystem)
		);
		auto modelPtr = model.get();

		auto view = std::make_unique<View>();

		auto controller = std::make_unique<Controller>(
			std::move(model),
			std::move(view)
		);

		controller->run();

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown error" << std::endl;
		return 1;
	}
	return 0;
}