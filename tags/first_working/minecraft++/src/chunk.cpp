#include <iostream>

#include <zlib.h>
#include <cassert>
#include <cstdlib>

#include "chunk.h"
#include "chunk_data.h"

#include "nbt.h"
#include "types.h"
#include "util.h"

using namespace minecraftpp;

Chunk::Chunk(ChunkData* pointer) : Counter(pointer) , chunkPointer(pointer) {
	setupPointers();
};

Chunk::Chunk(const Chunk &temp ) : Counter(temp) , chunkPointer(temp.chunkPointer) {
	setupPointers();
}

void Chunk::setupPointers() {
	TagByteArray* blockarraytag = dynamic_cast<TagByteArray*>(chunkPointer->getTag(BLOCKS));	
	blockPointer = reinterpret_cast<uint8_t (*)[CHUNKZ][CHUNKY]>(blockarraytag->data);	
}

void Chunk::modified() {
	free(chunkPointer->gzipData);
	chunkPointer->gzipData = NULL;
	chunkPointer->modified = true;
}

void Chunk::swap(Chunk &B) {
	std::swap(blockPointer,B.blockPointer);
	std::swap(chunkPointer,B.chunkPointer);	
}

Chunk& Chunk::operator=(Chunk B) {
	swap(B);
	return *this;
}

BlockType Chunk::getBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos) const {
	return static_cast<BlockType>(blockPointer[xPos][zPos][yPos]);
}

void Chunk::setBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos,BlockType input) {
	modified();
	blockPointer[xPos][zPos][yPos] = static_cast<uint8_t>(input);
}

uint8_t Chunk::getBlockData(uint8_t xPos,uint8_t yPos,uint8_t zPos) const {
	TagByteArray* blockdataarraytag = dynamic_cast<TagByteArray*>(chunkPointer->getTag(DATA));
	unsigned int index = xPos*CHUNKZ*CHUNKY + yPos + zPos*CHUNKY;
	uint8_t blockdata = blockdataarraytag->data[index>>1];
	return static_cast<uint8_t>(((index%2)?(blockdata>>4):(blockdata))&0x0F);	
}
		
