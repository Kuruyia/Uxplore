#ifndef NAVIGATIONHISTORY_HPP
#define NAVIGATIONHISTORY_HPP

#include <string>
#include <vector>

class NavigationHistory {
public:
	NavigationHistory();

	void go(std::string dirName);

	std::string getCurrentPathAsString();

	std::vector<std::string> *getHistory();
	unsigned int getHistoryPosition();

	void back();
	void forward();
private:
	std::vector<std::string> m_history;
	unsigned int m_historyPosition;
};

#endif // NAVIGATIONHISTORY_HPP