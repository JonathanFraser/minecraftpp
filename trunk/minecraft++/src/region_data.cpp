#include <iostream>

#include <cassert>
#include <time.h>
#include <stdlib.h>

#include "util.h"
#include "region_data.h"

namespace minecraftpp {

RegionData::RegionData(std::string filename) : regionFile(filename.c_str()) {
	//parse filename to get X and Y
	size_t start = filename.find("r.");
	size_t stop = filename.find(".mcr");
	std::string nums =filename.substr(start+2,stop-start-2);
	size_t dotpos = nums.find(".");
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

	regionFile.read(reinterpret_cast<char*>(locbuf),SECTOR_SIZE);
	regionFile.read(reinterpret_cast<char*>(timebuf),SECTOR_SIZE);

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




Chunk RegionData::getChunk(uint8_t xPos,uint8_t zPos) {
	assert(xPos<REGIONX);
	assert(zPos<REGIONZ);
	if( !chunkInFile(xPos,zPos) ) {
		chunkTable[zPos][xPos] = new ChunkData(this);
		timeStamps[zPos][xPos] = time(NULL);
	}

	if(!chunkLoaded(xPos,zPos)) {
		assert(offsets[zPos][xPos] >= 2 && counts[zPos][xPos] > 0);
		regionFile.seekg(offsets[zPos][xPos]*SECTOR_SIZE,std::ios::beg);
		chunkTable[zPos][xPos] = new ChunkData(this);
		uint8_t* data = chunkTable[zPos][xPos]->getDataPointer(counts[zPos][xPos]);
		regionFile.read(reinterpret_cast<char *>(data),SECTOR_SIZE*counts[zPos][xPos]);
	}

	return Chunk(chunkTable[zPos][xPos]);
}

bool RegionData::chunkInFile(uint8_t xPos,uint8_t zPos) {
	return (offsets[zPos][xPos] || counts[zPos][xPos]);
}

bool RegionData::chunkLoaded(uint8_t xPos,uint8_t zPos) {
	return chunkTable[zPos][xPos] != NULL;
}


RegionData::~RegionData() {
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

}

