#include <stdint.h>
#include <string>
#include <fstream>
#include "ref_count.h"
#include "chunk.h"

#ifndef __REGION_H
#define __REGION_H

const uint8_t REGIONX = 32;
const uint8_t REGIONZ = 32;

class RegionData : public Countee {
	private:
		int64_t X;
		int64_t Y;

		uint32_t timeStamps[REGIONX][REGIONZ];
		uint32_t offsets[REGIONX][REGIONZ];
		uint8_t counts[REGIONX][REGIONZ];
		ChunkData* chunkTable[REGIONX][REGIONZ];
		
		std::ifstream regionFile;
		void readRegionData();
		void thaw() {};
		void freeze();

		RegionData(const RegionData &) = delete;
		void operator=(const RegionData &) = delete;	

	public:
		RegionData(std::string filename);
		ChunkInterface getChunk(uint8_t xPos,uint8_t zPos);
		bool chunkInFile(uint8_t xPos,uint8_t zPos);
		bool chunkLoaded(uint8_t xPos,uint8_t zPos);
		~RegionData();
};

class RegionInterface : public Counter {
	private:
		friend class World;
		RegionData* pointer;
		RegionInterface(RegionData* pointer);
		void swap(RegionInterface &);
	public:
		RegionInterface(const RegionInterface&);
		RegionInterface & operator=(RegionInterface);
		ChunkInterface getChunk(uint8_t xPos,uint8_t zPos) const;
		bool chunkInFile(uint8_t xPos,uint8_t zPos) const ;
		bool chunkLoaded(uint8_t xPos,uint8_t zPos);
};
#endif
