#ifndef __REGION_H
#define __REGION_H

#include <stdint.h>
#include <string>
#include <fstream>
#include "ref_count.h"
#include "chunk.h"

namespace minecraftpp {

class RegionData;

class Region : public Counter {
	private:
		friend class World;
		friend class ChunkData;
		RegionData* pointer;
		Region(RegionData* pointer);
		void swap(Region &);
	public:
		Region(const Region&);
		Region & operator=(Region);
		Chunk getChunk(uint8_t xPos,uint8_t zPos) const;
		bool chunkInFile(uint8_t xPos,uint8_t zPos) const ;
		bool chunkLoaded(uint8_t xPos,uint8_t zPos);
};

}
#endif
