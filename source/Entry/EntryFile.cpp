#include "EntryFile.hpp"

EntryFile::EntryFile(File* file)
: m_file(file)
{}

std::string EntryFile::getText() {
	return m_file->getName();
}

Entry::EntryType EntryFile::getType() {
	return EntryType::ENTRY_FILE;
}

std::string EntryFile::getIconName() {
	return "file.png";
}