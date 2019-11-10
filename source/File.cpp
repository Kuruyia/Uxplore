#include "File.hpp"

File::File(const std::string& name)
: m_name(name)
{}

std::string File::getName() {
	return m_name;
}