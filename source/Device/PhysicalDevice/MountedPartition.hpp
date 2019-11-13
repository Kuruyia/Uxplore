#ifndef UXPLORE_CPP_MOUNTEDPARTITION_HPP
#define UXPLORE_CPP_MOUNTEDPARTITION_HPP

#include <FilesystemProvider.hpp>

class MountedPartition: public FilesystemProvider {
public:
    enum Filesystem {
        Unknown,

        FAT,
        Native
    };

    MountedPartition(const std::string &id, Filesystem filesystem);

    const std::string &getId() const;
    const Filesystem getFilesystem() const;

    const std::string &getName() const;
    void setName(const std::string &name);

    std::shared_ptr<File> getFile(std::string path) override;
    std::vector<std::shared_ptr<File>> listFolder(std::string path) override;
    void writeFile(std::string path) override;
    void readFile(std::string path) override;
    void deleteEntry(std::string path) override;
    void createFolder(std::string path) override;

private:
    const std::string m_id;
    const Filesystem m_filesystem;
    std::string m_name;
};


#endif //UXPLORE_CPP_MOUNTEDPARTITION_HPP
