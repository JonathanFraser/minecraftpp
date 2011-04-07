#ifndef __REGION_INTERFACE_H
#define __REGION_INTERFACE_H

#include <stdint.h>
#include <string>
#include <fstream>
#include "ref_count.h"
#include "chunk.h"
#include "chunk_data.h"

class RegionData;

class RegionInterface : public Counter {
	private:
		friend class World;
		friend class ChunkData;
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
