#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <string>

class Entry {
public:
	enum EntryType {
		ENTRY_UNKNOWN,

		ENTRY_FOLDER,
		ENTRY_FILE,
		ENTRY_PARTITION
	};

	virtual std::string getText() const = 0;
	virtual EntryType getType() const = 0;
	virtual std::string getIconName() const = 0;

	virtual std::unique_ptr<Entry> clone() const = 0;
};

#endif // ENTRYITEM_HPP