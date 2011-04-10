#ifndef __CHUNK_H
#define __CHUNK_H

#include <stdint.h>
#include "ref_count.h"
#include "types.h"
#include "nbt.h"

namespace minecraftpp {

class ChunkData;
class Chunk : public Counter {
	private:
		friend class RegionData;
		ChunkData* chunkPointer;
		Chunk(ChunkData* pointer);
		uint8_t (*blockPointer)[CHUNKZ][CHUNKY];
		void modified();
		void setupPointers();
		void swap( Chunk &);
	public:
		Chunk& operator=(Chunk);

		Chunk(const Chunk&);
		void setBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos,BlockType block);	
		BlockType getBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos) const;
		uint8_t getBlockData(uint8_t xPos,uint8_t yPos,uint8_t zPos) const;
};

}

#endif
