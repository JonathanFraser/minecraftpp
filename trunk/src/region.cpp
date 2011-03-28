#include <iostream>

#include <cassert>
#include <time.h>
#include <stdlib.h>

#include "util.h"
#include "region.h"

RegionData::RegionData(std::string filename) {
	//parse filename to get X and Y
	unsigned int start = filename.find("r.");
	unsigned int stop = filename.find(".mcr");
	regionFile = new std::ifstream(filename.c_str(),std::ios_base::in);
	std::string nums =filename.substr(start+2,stop-start-2);
	unsigned int dotpos = nums.find(".");
	std::string Xstr = nums.substr(0,dotpos);
	std::string Ystr = nums.substr(dotpos+1,nums.length() - dotpos-1);
	X=atoll(Xstr.c_str());
	Y=atoll(Ystr.c_str());
	readRegionData();
}

void RegionData::readRegionData() {
	uint8_t *locbuf = new uint8_t[SECTOR_SIZE];
	uint8_t *timebuf = new uint8_t[SECTOR_SIZE];
	uint8_t *locbuf2 = locbuf;
	uint8_t *timebuf2 = timebuf;

	regionFile->seekg(0,std::ios::end);
	uint64_t length = regionFile->tellg();
	regionFile->seekg(0,std::ios::beg);

	regionFile->read(reinterpret_cast<char*>(locbuf),1024*sizeof(uint32_t));
	regionFile->read(reinterpret_cast<char*>(timebuf),1024*sizeof(uint32_t));
	length -= 2* 1024*sizeof(uint32_t);


	for(int i=0;i<REGIONX;i++) {
		for(int j=0;j<REGIONZ;j++) {
			timeStamps[i][j] = arrayToInt(timebuf2);
			uint32_t temp = arrayToInt(locbuf2);
			counts[i][j] = (temp)&0x000000FF;
			offsets[i][j] = (temp)>>8;
			chunkTable[i][j]=NULL;
		}
	}
	delete[] locbuf;
	delete[] timebuf;	
}

ChunkInterface RegionData::getChunk(uint8_t xPos,uint8_t zPos) {
	assert(xPos<REGIONX);
	assert(zPos<REGIONZ);
	if( !chunkInFile(xPos,zPos) ) {
		chunkTable[xPos][zPos] = new ChunkData();
		timeStamps[xPos][zPos] = time(NULL);
	}

	if(!chunkLoaded(xPos,zPos)) {
		regionFile->seekg(offsets[xPos][zPos]*SECTOR_SIZE,std::ios::beg);
		chunkTable[xPos][zPos] = new ChunkData();
		uint8_t* data = chunkTable[xPos][zPos]->getDataPointer(counts[xPos][zPos]);
		regionFile->read(reinterpret_cast<char *>(data),SECTOR_SIZE*counts[xPos][zPos]);
	}

	return ChunkInterface(chunkTable[xPos][zPos]);
}

bool RegionData::chunkInFile(uint8_t xPos,uint8_t zPos) {
	return (offsets[xPos][zPos] || counts[xPos][zPos] || timeStamps[xPos][zPos]);
}

bool RegionData::chunkLoaded(uint8_t xPos,uint8_t zPos) {
	return chunkTable[xPos][zPos] != NULL;
}


RegionData::~RegionData() {
	delete regionFile;
	for(unsigned int i = 0;i<REGIONX;i++)
		for(unsigned int j = 0;j<REGIONZ;j++)
			delete chunkTable[i][j];

}

void RegionData::freeze() {
	for(unsigned int i = 0;i<REGIONX;i++)
		for(unsigned int j = 0;j<REGIONZ;j++)
			if(chunkTable[i][j] != NULL && !(chunkTable[i][j]->isModified())) {
				delete chunkTable[i][j];
				chunkTable[i][j] = NULL;
			}
}

RegionInterface::RegionInterface(RegionData* pointer) : Counter(pointer) , pointer(pointer){
}

RegionInterface::RegionInterface(const RegionInterface &B) : Counter(B.data) , pointer(B.pointer){
}

ChunkInterface RegionInterface::getChunk(uint8_t xPos,uint8_t zPos) {
	return pointer->getChunk(xPos,zPos);
}

bool RegionInterface::chunkInFile(uint8_t xPos,uint8_t zPos) {
	return pointer->chunkInFile(xPos,zPos);
}

bool RegionInterface::chunkLoaded(uint8_t xPos,uint8_t zPos) {
	return pointer->chunkLoaded(xPos,zPos);
}
