#include <iostream>

#include <zlib.h>
#include <cassert>
#include <cstdlib>

#include "chunk_interface.h"
#include "chunk_data.h"

#include "nbt.h"
#include "util.h"

const std::string LEVEL = "Level";
const std::string BLOCKS = "Blocks";
const std::string BLOCK_DATA = "Data";
const std::string SKY_LIGHT = "SkyLight";
const std::string BLOCK_LIGHT = "BlockLight";
const std::string HEIGHT_MAP = "HeightMap";
const std::string ENTITIES = "Entities";
const std::string TILE_ENTITIES = "TileEntities";
const std::string LAST_UPDATE ="LastUpdate";
const std::string X_POS = "xPos";
const std::string Z_POS = "zPos";
const std::string TERRAIN_POPULATED = "TerrainPopulated";


ChunkInterface::ChunkInterface(ChunkData* pointer) : Counter(pointer) , chunkPointer(pointer) {
	setupPointers();
};

ChunkInterface::ChunkInterface(const ChunkInterface &temp ) : Counter(temp) , chunkPointer(temp.chunkPointer) {
	setupPointers();
}

void ChunkInterface::setupPointers() {
	TagByteArray* blockarraytag = dynamic_cast<TagByteArray*>(chunkPointer->getTag(BLOCKS));	
	blockPointer = reinterpret_cast<uint8_t (*)[CHUNKZ][CHUNKY]>(blockarraytag->data);	
}

void ChunkInterface::modified() {
	free(chunkPointer->gzipData);
	chunkPointer->gzipData = NULL;
	chunkPointer->modified = true;
}

void ChunkInterface::swap(ChunkInterface &B) {
	std::swap(blockPointer,B.blockPointer);
	std::swap(chunkPointer,B.chunkPointer);	
}

ChunkInterface& ChunkInterface::operator=(ChunkInterface B) {
	swap(B);
	return *this;
}

BlockType ChunkInterface::getBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos) const {
	return static_cast<BlockType>(blockPointer[xPos][zPos][yPos]);
}

void ChunkInterface::setBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos,BlockType input) {
	modified();
	blockPointer[xPos][zPos][yPos] = input;
}

uint8_t ChunkInterface::getBlockData(uint8_t xPos,uint8_t yPos,uint8_t zPos) const {
	TagByteArray* blockdataarraytag = dynamic_cast<TagByteArray*>(chunkPointer->getTag(BLOCK_DATA));
	unsigned int index = xPos*CHUNKZ*CHUNKY + yPos + zPos*CHUNKY;
	uint8_t blockdata = blockdataarraytag->data[index>>1];
	return ((index%2)?(blockdata>>4):(blockdata))&0x0F;	
}
		
