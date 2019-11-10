#ifndef DISCINTERFACEWRAPPER_H
#define DISCINTERFACEWRAPPER_H

#include "DiscInterface.hpp"

class DiscInterfaceWrapper {
public:
	static bool startup(void* data);
	static bool isInserted(void* data);
	static bool clearStatus(void* data);
	static bool shutdown(void* data);
	static bool readSectors(void* data, uint32_t sector, uint32_t numSectors, void* buffer);
	static bool writeSectors(void* data, uint32_t sector, uint32_t numSectors, const void* buffer);

private:
	static DiscInterface* getInterfaceFromData(void* data);
};

#endif // DISCINTERFACEWRAPPER_H