#include "NavigationHistory.hpp"

NavigationHistory::NavigationHistory()
: m_historyPosition(0)
{}

void NavigationHistory::go(std::string dirName) {
	if (m_historyPosition < m_history.size())
		m_history.erase(m_history.begin() + m_historyPosition, m_history.end());

	m_history.emplace_back(dirName);
	m_historyPosition = m_history.size();
}

std::string NavigationHistory::getCurrentPathAsString() {
	std::string currentPath;
	
	for (int i = 0; i < m_historyPosition; i++)
		currentPath.append(m_history.at(i)).append("/");

	return currentPath;
}

std::vector<std::string> *NavigationHistory::getHistory() {
	return &m_history;
}

unsigned int NavigationHistory::getHistoryPosition() {
	return m_historyPosition;
}

void NavigationHistory::back() {
	if (m_historyPosition > 0)
		m_historyPosition--;
}

void NavigationHistory::forward() {
	if (m_historyPosition < m_history.size())
		m_historyPosition++;
}