#include "MountedPartition.hpp"

MountedPartition::MountedPartition(const std::string &id, Filesystem filesystem)
: m_id(id)
, m_name("<MISSINGNO>")
, m_filesystem(filesystem) {

}

std::shared_ptr<File> MountedPartition::getFile(std::string path) {
    return std::shared_ptr<File>();
}

std::vector<std::shared_ptr<File>> MountedPartition::listFolder(std::string path) {
    return std::vector<std::shared_ptr<File>>();
}

void MountedPartition::writeFile(std::string path) {

}

void MountedPartition::readFile(std::string path) {

}

void MountedPartition::deleteEntry(std::string path) {

}

void MountedPartition::createFolder(std::string path) {

}

const std::string &MountedPartition::getName() const {
    return m_name;
}

void MountedPartition::setName(const std::string &name) {
    m_name = name;
}

const std::string &MountedPartition::getId() const {
    return m_id;
}

const MountedPartition::Filesystem MountedPartition::getFilesystem() const {
    return m_filesystem;
}
