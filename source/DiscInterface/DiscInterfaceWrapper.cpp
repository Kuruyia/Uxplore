#include "DiscInterfaceWrapper.hpp"

bool DiscInterfaceWrapper::startup(void* data) {
    return getInterfaceFromData(data)->startup();
}

bool DiscInterfaceWrapper::isInserted(void* data) {
    return getInterfaceFromData(data)->isInserted();
}

bool DiscInterfaceWrapper::clearStatus(void* data) {
    return getInterfaceFromData(data)->clearStatus();
}

bool DiscInterfaceWrapper::shutdown(void* data) {
    return getInterfaceFromData(data)->shutdown();
}

bool DiscInterfaceWrapper::readSectors(void* data, uint32_t sector, uint32_t numSectors, void* buffer) {
    return getInterfaceFromData(data)->readSectors(sector, numSectors, buffer);
}

bool DiscInterfaceWrapper::writeSectors(void* data, uint32_t sector, uint32_t numSectors, const void* buffer) {
    return getInterfaceFromData(data)->writeSectors(sector, numSectors, buffer);
}

DiscInterface* DiscInterfaceWrapper::getInterfaceFromData(void* data) {
	return static_cast<DiscInterface*>(data);
}