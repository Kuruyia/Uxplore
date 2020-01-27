/*
    Uxplore
    Copyright (C) 2019-2019, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UXPLORE_CPP_MOUNTEDPARTITION_H
#define UXPLORE_CPP_MOUNTEDPARTITION_H

#include <FilesystemProvider.h>

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


#endif //UXPLORE_CPP_MOUNTEDPARTITION_H
