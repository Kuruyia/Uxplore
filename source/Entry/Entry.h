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

#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <string>

class Entry {
public:
	enum EntryType {
		ENTRY_UNKNOWN,

		ENTRY_FOLDER,
		ENTRY_FILE,
		ENTRY_PARTITION
	};

	virtual ~Entry() = default;

	virtual std::string getText() const = 0;
	virtual EntryType getType() const = 0;
	virtual std::string getIconName() const = 0;

	virtual std::unique_ptr<Entry> clone() const = 0;
};

#endif // ENTRYITEM_HPP