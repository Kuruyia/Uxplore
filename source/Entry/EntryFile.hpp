#ifndef ENTRYFILE_HPP
#define ENTRYFILE_HPP

#include <memory>
#include "Entry.hpp"
#include "../File.hpp"

class EntryFile : public Entry {
public:
	EntryFile(std::shared_ptr<File>& file);

	std::string getText() override;
	EntryType getType() override;
	std::string getIconName() override;

private:
    std::shared_ptr<File> m_file;
};

#endif // ENTRYFILE_HPP