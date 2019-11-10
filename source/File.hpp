#ifndef FILE_HPP
#define FILE_HPP

#include <string>

class File {
public:
	File(std::string name);

	std::string getName();

protected:
	std::string m_name;
};

#endif // FILE_HPP