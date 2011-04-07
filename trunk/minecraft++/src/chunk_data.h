#ifndef __CHUNK_DATA_H
#define __CHUNK_DATA_H

#include <stdint.h>
#include "chunk.h"
#include "ref_count.h"
#include "nbt.h"

class RegionData;
class RegionInterface;

/**
* @class ChunkData
* @brief Class to represent a chunk of the world
*	
*	This Class holds a chunk of world data to use this class
* To create a new chunk use the default constructer and edit
* to your hearts content via the interface class
* To load a compressed chunk
* geta pointer of the appropriate size from getDataPointer and copy
* the gzipped data in to that memory
**/
class ChunkData : public Countee {
	private:
		uint8_t *gzipData;
		uint32_t sectorCount;
		uint32_t length;
		uint8_t compressionType;
		bool modified;
		RegionInterface* parentInterface;
		RegionData* parent;
		nbtFile* chunkFile;
		ChunkData(const ChunkData&) = delete;
		ChunkData& operator=(const ChunkData&) = delete;
		friend class ChunkInterface;
		Tag* getTag(const std::string &);	
	public:
		ChunkData(RegionData *parent);
		
		/**
		* Forces the data to decompress if a pointer was grabbed
		* or to creates a new level if not
		*/
		void load();

		/**
		* grabs a data pointer to memory, size is 4k*sectorCount
		*/
		uint8_t* getDataPointer(uint32_t sectorCount);

		
		bool isModified();

		void thaw();
		void freeze();	
		~ChunkData();
		
};
#endif
