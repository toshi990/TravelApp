#include "AExcursion.h"
#include <stdexcept>

std::string Excursion::getExcursionName() const noexcept { return excursionName; }
std::chrono::year_month_day Excursion::getStartDate() const noexcept { return startDate; }
double Excursion::getPrice() const noexcept { return price; }
size_t Excursion::getMaxParticipants() const noexcept { return maxParticipants; }
size_t Excursion::getParticipantsCount() const noexcept { return participantsCount; }
std::string Excursion::getID() const noexcept { return ID; }

void Excursion::setParticipantsCount(std::size_t count) {
	if (count <= maxParticipants) {
		participantsCount = count;
	}
	else {
		throw std::out_of_range("Amount of participants is out of range");
	}
}

void Excursion::setStartDate(std::chrono::year_month_day nweDate) {
	startDate = nweDate;
}

void Excursion::modifyParticipantsCount(int change) {
	int newCount = participantsCount + change;
	if (newCount <= maxParticipants && newCount >= 0) {
		participantsCount = newCount;
	}
	else {
		throw std::out_of_range("Amount of participants is out of range");
	}
}