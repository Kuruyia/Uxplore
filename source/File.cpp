#include "File.hpp"

#include <utility>

File::File(std::string name)
: m_name(std::move(name))
{}

std::string File::getName() {
	return m_name;
}