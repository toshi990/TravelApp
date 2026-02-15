#ifndef TRAVEL_APP_EXCURSIONKEY_H
#define TRAVEL_APP_EXCURSIONKEY_H

#include <string>
#include <functional>

/**
 * @brief Key class used for identifying excursions in hashed containers.
 * * This class encapsulates the unique string ID of an excursion to be used as a key
 * * in repositories or maps.
 */
class ExcursionKey {
private:
	std::string ID; ///< The unique identifier string.

public:
	/**
	 * @brief Constructs an ExcursionKey with a specific ID.
	 * @param id The unique string identifier.
	 */
	ExcursionKey(const std::string& id) : ID(id) {}

	/**
	 * @brief Equality operator for comparing two keys.
	 * @param other The other key to compare with.
	 * @return true if the IDs are identical, false otherwise.
	 */
	bool operator==(const ExcursionKey &other) const {
		return ID == other.ID;
	}

	/**
	 * @brief Getter for the internal ID string.
	 * @return A constant reference to the ID string.
	 */
	const std::string& getID() const { return ID; }
};

/**
 * @brief Specialization of std::hash for ExcursionKey.
 * * Allows ExcursionKey to be used as a key in unordered containers like std::unordered_map.
 */
template <>
struct std::hash<ExcursionKey> {
	/**
	 * @brief Generates a hash value for an ExcursionKey.
	 * @param k The key object to hash.
	 * @return The calculated hash size_t value.
	 */
	size_t operator()(const ExcursionKey &k) const {
		return std::hash<std::string>{}(k.getID());
	}
};

#endif //TRAVEL_APP_EXCURSIONKEY_H