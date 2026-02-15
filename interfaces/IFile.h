#ifndef SUCCESS_IFILE_H
#define SUCCESS_IFILE_H
#include "IExcursionRepository.h"

class IFile {
	public:
	virtual ~IFile() = default;
	virtual void save(const std::string& filename, IExcursionRepository* repository) = 0;
	virtual void load(const std::string& filename, IExcursionRepository* repository) = 0;
};

#endif //SUCCESS_IFILE_H