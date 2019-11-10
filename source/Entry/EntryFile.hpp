#ifndef ENTRYFILE_HPP
#define ENTRYFILE_HPP

#include "Entry.hpp"
#include "../File.hpp"

class EntryFile : public Entry {
public:
	EntryFile(File* file);

	std::string getText() override;
	EntryType getType() override;
	std::string getIconName() override;

private:
	File* m_file;
};

#endif // ENTRYFILE_HPP