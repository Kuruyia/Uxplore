#include "LocalFile.hpp"

LocalFile::LocalFile(std::string name, std::string path, FilesystemProvider* device)
: File(name)
, m_path(path)
, m_device(device)
{}

std::string LocalFile::getPath() {
	return m_path;
}

FilesystemProvider* LocalFile::getDevice() {
	return m_device;
}