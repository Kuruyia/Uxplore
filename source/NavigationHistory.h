/*
    Uxplore
    Copyright (C) 2019-2020, Kuruyia

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

#ifndef NAVIGATIONHISTORY_HPP
#define NAVIGATIONHISTORY_HPP

#include <string>
#include <vector>

class NavigationHistory {
public:
    NavigationHistory();

    void go(const std::string &dirName);

    std::string getCurrentPathAsString();
    const std::vector<std::string> & getHistory();
    const size_t & getHistoryPosition();

    void back();
    void forward();

private:
    std::vector<std::string> m_history;
    size_t m_historyPosition;
};

#endif // NAVIGATIONHISTORY_HPP