#include "MountedPartition.hpp"

MountedPartition::MountedPartition(const std::string &partitionId)
: m_partitionId(partitionId)
{

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