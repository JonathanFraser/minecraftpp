#include <iostream>

#include <zlib.h>
#include <cassert>
#include <cstdlib>

#include "chunk_data.h"
#include "region_data.h"
#include "region.h"
#include "types.h"

#include "nbt.h"
#include "util.h"

using namespace minecraftpp;

ChunkData::ChunkData(RegionData* parent) : gzipData(NULL), sectorCount(0),length(0),compressionType(0),modified(false), parentInterface(NULL),parent(parent),chunkFile(NULL){

}

ChunkData::~ChunkData() {
	delete chunkFile;
	free(gzipData);
}

uint8_t* ChunkData::getDataPointer(uint32_t sectorCount) {
	assert(gzipData == NULL);
	this->sectorCount = sectorCount;
	gzipData = static_cast<uint8_t*>(malloc(sectorCount*SECTOR_SIZE));
	return gzipData;
}


Tag* ChunkData::getTag(const std::string& name) {
	return dynamic_cast<TagCompound*>(chunkFile->getRootNode()->getSubTag(LEVEL))->getSubTag(name);
}

bool ChunkData::isModified() {
	return modified;
}

void ChunkData::thaw() {
	
	assert(chunkFile == NULL);
	
	if(gzipData == NULL) {
		//the datapointer to compressed data is null so create a new chunk
		modified=true;
		chunkFile = new nbtFile();
		TagCompound* newchunk = new TagCompound();
		newchunk->insertSubTag(BLOCKS,new TagByteArray(32768));
		newchunk->insertSubTag(DATA,new TagByteArray(16384));
		newchunk->insertSubTag(SKY_LIGHT,new TagByteArray(16384));
		newchunk->insertSubTag(LIGHT,new TagByteArray(16384));
		newchunk->insertSubTag(HEIGHT_MAP,new TagByteArray(256));
		newchunk->insertSubTag(ENTITIES,new TagList());
		newchunk->insertSubTag(TILE_ENTITIES,new TagList());
		newchunk->insertSubTag(LAST_UPDATE,new TagLong());
		newchunk->insertSubTag(X_POS,new TagInt);
		newchunk->insertSubTag(Z_POS,new TagInt());
		newchunk->insertSubTag(TERRAIN_POPULATED,new TagByte());
		chunkFile->getRootNode()->insertSubTag(LEVEL,newchunk);
	} else {
		//the standard says a chunk will always be less then a mega so grab that
		uint8_t *outputdata = new uint8_t[MEGABYTE];
		long unsigned int buffersize = MEGABYTE;
		uint8_t *data2=gzipData;
		
		//grab the chunk header info
		length=arrayToInt(data2);
		assert(sectorCount - (length + 5)/SECTOR_SIZE <= 1);
		compressionType = *data2++;
		
		//such a simple decompressor
		uncompress(outputdata,&buffersize,data2,length);

		//this parses the file data in and NBT tree
		chunkFile = new nbtFile(outputdata);
		
		//we done with the crap so get rid of it
		delete[] outputdata;	
	}
	assert(parentInterface == NULL);
	parentInterface = new RegionInterface(parent);
}

void ChunkData::freeze() {
	if(gzipData == NULL) {
		gzipData = new uint8_t[MEGABYTE];
		uint8_t* rawData = new uint8_t[MEGABYTE];
		long unsigned int datasize = chunkFile->write(rawData,sizeof(rawData));
		delete chunkFile;
		chunkFile = NULL;
		long unsigned int destsize = MEGABYTE-5;
		compress(gzipData+5,&destsize,rawData,datasize);
		delete rawData;
		uint8_t* temp = gzipData;
		intToArray(temp,static_cast<int32_t>(destsize));
		*temp++=1;
		destsize = destsize+5;
		sectorCount = destsize/SECTOR_SIZE + (destsize%SECTOR_SIZE)?1:0;
		gzipData = static_cast<uint8_t*>(realloc(gzipData,sectorCount*SECTOR_SIZE));	
	} else {
		delete chunkFile;
		chunkFile=NULL;
	}
	delete parentInterface;
	parentInterface = NULL;
}

