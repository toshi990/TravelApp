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
/**
 * @brief Hybrid Hash Function (Optimization for Analytics)
 * * Applies different algorithms depending on the string type.
 */
template <>
struct std::hash<ExcursionKey> {
	size_t operator()(const ExcursionKey &k) const {
		const std::string& id = k.getID();

		if (id.length() < 15) {
			size_t hash = 2166136261u;
			for (char c : id) {
				hash ^= static_cast<size_t>(c);
				hash *= 16777619u;
			}
			return hash;
		}
		else {
			return std::hash<std::string>{}(id);
		}
	}
};

#endif //TRAVEL_APP_EXCURSIONKEY_H