#include <iostream>
#include <chrono>
#include "excursionRepository.h"

void runAnalytics() {
	HashTableRepository repo(10000);

	for(int i = 0; i < 50000; ++i) {
		repo.addExcursion(std::make_unique<ExcursionModel>("ID-" + std::to_string(i))); // Короткие
		repo.addExcursion(std::make_unique<ExcursionModel>("/route/europe/france/paris/tour-variant-" + std::to_string(i))); // Длинные
	}

	auto stats = repo.getTableStats();

	std::cout << "--- Analytics Report ---\n";
	std::cout << "Total Elements: " << stats.totalElements << "\n";
	std::cout << "Collisions: " << stats.collisions << "\n";
	std::cout << "Max Bucket Chain: " << stats.maxBucketSize << "\n";
}