#include "region.h"
#include "chunk_data.h"
#include "region_data.h"

#include "util.h"
#include <algorithm>

namespace minecraftpp {

RegionInterface::RegionInterface(RegionData* pointer) : Counter(pointer) , pointer(pointer){
}

RegionInterface::RegionInterface(const RegionInterface &B) : Counter(B.data) , pointer(B.pointer){
}

void RegionInterface::swap(RegionInterface &B) {
	Counter::swap(B);
	std::swap(pointer,B.pointer);
}

RegionInterface& RegionInterface::operator=(RegionInterface B) {
	swap(B);
	return *this;
}

ChunkInterface RegionInterface::getChunk(uint8_t xPos,uint8_t zPos) const {
	return pointer->getChunk(xPos,zPos);
}

bool RegionInterface::chunkInFile(uint8_t xPos,uint8_t zPos) const {
	return pointer->chunkInFile(xPos,zPos);
}

bool RegionInterface::chunkLoaded(uint8_t xPos,uint8_t zPos) {
	return pointer->chunkLoaded(xPos,zPos);
}
}
