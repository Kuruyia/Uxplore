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

#include "NavigationHistory.h"

NavigationHistory::NavigationHistory()
        : m_historyPosition(0)
{}

void NavigationHistory::go(const std::string &dirName)
{
    if (m_historyPosition < m_history.size())
        m_history.erase(m_history.begin() + m_historyPosition, m_history.end());

    m_history.emplace_back(dirName);
    m_historyPosition = m_history.size();
}

std::string NavigationHistory::getCurrentPathAsString()
{
    std::string currentPath;

    for (int i = 0; i < m_historyPosition; i++)
        currentPath.append(m_history.at(i)).append("/");

    return currentPath;
}

const std::vector<std::string> & NavigationHistory::getHistory()
{
    return m_history;
}

const size_t & NavigationHistory::getHistoryPosition()
{
    return m_historyPosition;
}

void NavigationHistory::back()
{
    if (m_historyPosition > 0)
        m_historyPosition--;
}

void NavigationHistory::forward()
{
    if (m_historyPosition < m_history.size())
        m_historyPosition++;
}