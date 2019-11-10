#ifndef LOCALFILE_H
#define LOCALFILE_H

#include "FilesystemProvider.hpp"
#include "File.hpp"

class LocalFile: public File {
public:
	LocalFile(std::string name, std::string path, FilesystemProvider* device);

	std::string getPath();

	FilesystemProvider* getDevice();

private:
	std::string m_path;

	FilesystemProvider* m_device;
};

#endif // LOCALFILE_H