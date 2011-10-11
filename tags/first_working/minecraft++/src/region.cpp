#include "region.h"
#include "chunk_data.h"
#include "region_data.h"

#include "util.h"
#include <algorithm>

namespace minecraftpp {

Region::Region(RegionData* pointer) : Counter(pointer) , pointer(pointer){
}

Region::Region(const Region &B) : Counter(B.data) , pointer(B.pointer){
}

void Region::swap(Region &B) {
	Counter::swap(B);
	std::swap(pointer,B.pointer);
}

Region& Region::operator=(Region B) {
	swap(B);
	return *this;
}

Chunk Region::getChunk(uint8_t xPos,uint8_t zPos) const {
	return pointer->getChunk(xPos,zPos);
}

bool Region::chunkInFile(uint8_t xPos,uint8_t zPos) const {
	return pointer->chunkInFile(xPos,zPos);
}

bool Region::chunkLoaded(uint8_t xPos,uint8_t zPos) {
	return pointer->chunkLoaded(xPos,zPos);
}
}
