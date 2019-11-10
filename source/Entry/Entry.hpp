#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <string>

class Entry {
public:
	enum EntryType {
		ENTRY_UNKNOWN,

		ENTRY_FOLDER,
		ENTRY_FILE,
		ENTRY_DEVICE
	};

	virtual std::string getText() = 0;
	virtual EntryType getType() = 0;
	virtual std::string getIconName() = 0;
};

#endif // ENTRYITEM_HPP