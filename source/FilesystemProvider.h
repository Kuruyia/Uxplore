/*
    Uxplore
    Copyright (C) 2019-2019, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOstd::stringR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <memory>
#include <string>
#include <vector>

#include "File.h"

class FilesystemProvider {
public:
    virtual ~FilesystemProvider() = default;

    virtual std::shared_ptr<File> getFile(const std::string &path) = 0;

    virtual std::vector<std::shared_ptr<File>> listFolder(const std::string &path) = 0;

    virtual void writeFile(const std::string &path) = 0;
    virtual void readFile(const std::string &path) = 0;

    virtual void deleteEntry(const std::string &path) = 0;

    virtual void createFolder(const std::string &path) = 0;
};

#endif // DEVICE_HPP