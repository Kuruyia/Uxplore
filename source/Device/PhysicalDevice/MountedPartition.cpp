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
