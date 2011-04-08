#ifndef __CHUNK_INTERFACE_H
#define __CHUNK_INTERFACE_H

#include <stdint.h>
#include "ref_count.h"
#include "types.h"
#include "nbt.h"

namespace minecraftpp {

class ChunkData;
class ChunkInterface : public Counter {
	private:
		friend class RegionData;
		ChunkData* chunkPointer;
		ChunkInterface(ChunkData* pointer);
		uint8_t (*blockPointer)[CHUNKZ][CHUNKY];
		void modified();
		void setupPointers();
		void swap( ChunkInterface &);
	public:
		ChunkInterface& operator=(ChunkInterface);

		ChunkInterface(const ChunkInterface&);
		void setBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos,BlockType block);	
		BlockType getBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos) const;
		uint8_t getBlockData(uint8_t xPos,uint8_t yPos,uint8_t zPos) const;
};

}

#endif
