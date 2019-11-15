#ifndef ENTRYFILE_HPP
#define ENTRYFILE_HPP

#include <memory>
#include "Entry.hpp"
#include "../File.hpp"

class EntryFile : public Entry {
public:
	EntryFile(std::shared_ptr<File>& file);

	std::string getText() const override;
	EntryType getType() const override;
	std::string getIconName() const override;

    std::unique_ptr<Entry> clone() const override;

private:
    std::shared_ptr<File> m_file;
};

#endif // ENTRYFILE_HPP