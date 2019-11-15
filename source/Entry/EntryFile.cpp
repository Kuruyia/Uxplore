#include "EntryFile.hpp"

EntryFile::EntryFile(std::shared_ptr<File>& file)
: m_file(file)
{}

std::string EntryFile::getText() const {
	return m_file->getName();
}

Entry::EntryType EntryFile::getType() const {
	return EntryType::ENTRY_FILE;
}

std::string EntryFile::getIconName() const {
	return "file.png";
}

std::unique_ptr<Entry> EntryFile::clone() const {
    return std::make_unique<EntryFile>(*this);
}
