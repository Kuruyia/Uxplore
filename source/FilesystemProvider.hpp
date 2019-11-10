#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <memory>
#include <string>
#include <vector>

#include "File.hpp"

class FilesystemProvider {
public:
	virtual std::shared_ptr<File> getFile(std::string path) = 0;
	virtual std::vector<std::shared_ptr<File>> listFolder(std::string path) = 0;

	virtual void writeFile(std::string path) = 0;
	virtual void readFile(std::string path) = 0;

	virtual void deleteEntry(std::string path) = 0;

	virtual void createFolder(std::string path) = 0;
};

#endif // DEVICE_HPP