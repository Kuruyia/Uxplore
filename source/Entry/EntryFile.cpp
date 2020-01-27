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

#include "EntryFile.h"

EntryFile::EntryFile(std::shared_ptr<File>& file)
: m_file(file)
{}

std::string EntryFile::getText() const {
	return m_file->getName();
}

Entry::EntryType EntryFile::getType() const {
	return EntryType::ENTRY_FILE;
}

std::string EntryFile::getIconName() const {
	return "files/file.png";
}

std::unique_ptr<Entry> EntryFile::clone() const {
    return std::make_unique<EntryFile>(*this);
}