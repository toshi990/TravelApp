#ifndef TRAVEL_APP_ITERATOR_H
#define TRAVEL_APP_ITERATOR_H

#include <vector>
#include "IExcursion.h"

/**
 * @brief Custom iterator for traversing a collection of excursions.
 * * This class wraps a standard vector iterator to provide a simplified
 * * interface for moving through IExcursion pointers.
 */
class ExcursionIterator {
private:
	std::vector<IExcursion*>::const_iterator current; ///< Internal standard iterator.

public:
	/**
	 * @brief Constructs an iterator from a standard vector iterator.
	 * @param it The starting position in the vector.
	 */
	ExcursionIterator(std::vector<IExcursion*>::const_iterator it) : current(it) {}

	/**
	 * @brief Advances the iterator to the next excursion.
	 * @return Reference to the updated iterator.
	 */
	ExcursionIterator& operator++() {
		++current;
		return *this;
	}

	/**
	 * @brief Accesses the excursion at the current position.
	 * @return Pointer to the current IExcursion object.
	 */
	IExcursion* operator*() const {
		return *current;
	}

	/**
	 * @brief Compares two iterators for inequality.
	 * * Used primarily to check if the iterator has reached the end of the collection.
	 * @param other The iterator to compare against.
	 * @return true if the iterators point to different positions, false otherwise.
	 */
	bool operator!=(const ExcursionIterator& other) const {
		return current != other.current;
	}
};

#endif // TRAVEL_APP_ITERATOR_H