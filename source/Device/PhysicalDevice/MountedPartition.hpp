#ifndef UXPLORE_CPP_MOUNTEDPARTITION_HPP
#define UXPLORE_CPP_MOUNTEDPARTITION_HPP


#include <FilesystemProvider.hpp>

class MountedPartition: public FilesystemProvider {
public:
    MountedPartition(const std::string &partitionId);

    std::shared_ptr<File> getFile(std::string path) override;
    std::vector<std::shared_ptr<File>> listFolder(std::string path) override;
    void writeFile(std::string path) override;
    void readFile(std::string path) override;
    void deleteEntry(std::string path) override;
    void createFolder(std::string path) override;

private:
    const std::string m_partitionId;
};


#endif //UXPLORE_CPP_MOUNTEDPARTITION_HPP
