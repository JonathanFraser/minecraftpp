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

	regionFile->read(reinterpret_cast<char*>(locbuf),SECTOR_SIZE);
	regionFile->read(reinterpret_cast<char*>(timebuf),SECTOR_SIZE);

	for(int j=0;j<REGIONZ;j++) {
		for(int i=0;i<REGIONX;i++) {
			timeStamps[i][j] = arrayToInt(timebuf2);
			uint32_t temp = arrayToInt(locbuf2);
			counts[j][i] = (temp)&0x000000FF;
			offsets[j][i] = (temp)>>8;
			chunkTable[j][i]=NULL;
		}
	}
	delete[] locbuf;
	delete[] timebuf;	
}

ChunkInterface RegionData::getChunk(uint8_t xPos,uint8_t zPos) {
	assert(xPos<REGIONX);
	assert(zPos<REGIONZ);
	if( !chunkInFile(xPos,zPos) ) {
		chunkTable[zPos][xPos] = new ChunkData();
		timeStamps[zPos][xPos] = time(NULL);
	}

	if(!chunkLoaded(xPos,zPos)) {
		regionFile->seekg(offsets[zPos][xPos]*SECTOR_SIZE,std::ios::beg);
		chunkTable[zPos][xPos] = new ChunkData();
		uint8_t* data = chunkTable[zPos][xPos]->getDataPointer(counts[zPos][xPos]);
		regionFile->read(reinterpret_cast<char *>(data),SECTOR_SIZE*counts[zPos][xPos]);
	}

	return ChunkInterface(chunkTable[zPos][xPos]);
}

bool RegionData::chunkInFile(uint8_t xPos,uint8_t zPos) {
	return (offsets[zPos][xPos] || counts[zPos][xPos] || timeStamps[zPos][xPos]);
}

bool RegionData::chunkLoaded(uint8_t xPos,uint8_t zPos) {
	return chunkTable[zPos][xPos] != NULL;
}


RegionData::~RegionData() {
	delete regionFile;
	for(unsigned int i = 0;i<REGIONX;i++)
		for(unsigned int j = 0;j<REGIONZ;j++)
			delete chunkTable[j][i];

}

void RegionData::freeze() {
	for(unsigned int i = 0;i<REGIONX;i++)
		for(unsigned int j = 0;j<REGIONZ;j++) {
			if(chunkTable[j][i] != NULL && chunkTable[j][i]->isModified()) {
#warning commit these chunks to a temp table
			} else {
				delete chunkTable[j][i];
				chunkTable[j][i] = NULL;
			}
		}
}

RegionInterface::RegionInterface(RegionData* pointer) : Counter(pointer) , pointer(pointer){
}

RegionInterface::RegionInterface(const RegionInterface &B) : Counter(B.data) , pointer(B.pointer){
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
