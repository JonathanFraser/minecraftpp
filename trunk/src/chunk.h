#ifndef __CHUNK_H
#define __CHUNK_H

#include <stdint.h>
#include "refcount.h"
const uint32_t MEGABYTE = 1024*1024;
const uint16_t SECTOR_SIZE = 4096;
const uint8_t CHUNKX = 16;
const uint8_t CHUNKY = 128;
const uint8_t CHUNKZ = 16;

enum BlockType {
	BLOCK_AIR, /* 0 */
	BLOCK_STONE,
	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_COBBLESTONE,
	BLOCK_PLANK,
	BLOCK_SAPLING,
	BLOCK_BEDROCK,
	BLOCK_WATER,
	BLOCK_STATIONARY_WATER,
	BLOCK_LAVA, /* 10 */
	BLOCK_STATIONARY_LAVA,
	BLOCK_SAND,
	BLOCK_GRAVEL,
	BLOCK_GOLD_ORE,
	BLOCK_IRON_ORE,
	BLOCK_COAL_ORE,
	BLOCK_WOOD,
	BLOCK_LEAVES,
	BLOCK_SPONGE,
	BLOCK_GLASS, /* 20 */
	BLOCK_LAPIS_ORE,
	BLOCK_LAPIS_BLOCK,
	BLOCK_DISPENSER,
	BLOCK_SANDSTONE,
	BLOCK_NOTE_BLOCK,
	BLOCK_BED, /* 26 */
	BLOCK_WOOL = 35,
	BLOCK_YELLOW_FLOWER = 37,
	BLOCK_ROSE,
	BLOCK_BROWN_MUSHROOM,
	BLOCK_RED_MUSHROOM, /* 40 */
	BLOCK_GOLD_BLOCK,
	BLOCK_IRON_BLOCK,
	BLOCK_DOUBLE_SLAB,
	BLOCK_SLAB,
	BLOCK_BRICK_BLOCK,
	BLOCK_TNT,
	BLOCK_BOOKSHELF,
	BLOCK_MOSSY_COBBLESTONE,
	BLOCK_OBSIDIAN,
	BLOCK_TORCH, /* 50 */
	BLOCK_FIRE,
	BLOCK_SPAWNER,
	BLOCK_WOODEN_STAIRS,
	BLOCK_CHEST,
	BLOCK_REDSTONE_WIRE,
	BLOCK_DIAMOND_ORE,
	BLOCK_DIAMOND_BLOCK,
	BLOCK_CRAFTING_TABLE,
	BLOCK_CROPS,
	BLOCK_FARMLAND, /* 60 */
	BLOCK_FURNACE,
	BLOCK_BURNING_FURNACE,
	BLOCK_SIGN_POST,
	BLOCK_WOODEN_DOOR,
	BLOCK_LADDER,
	BLOCK_RAILS,
	BLOCK_COBBLESTONE_STAIRS,
	BLOCK_WALL_SIGN,
	BLOCK_LEVER,
	BLOCK_STONE_PRESSURE_PLATE, /* 70 */
	BLOCK_IRON_DOOR,
	BLOCK_WOODEN_PRESSURE_PLATE,
	BLOCK_REDSTONE_ORE,
	BLOCK_GLOWING_REDSTONE_ORE,
	BLOCK_REDSTONE_TORCH_OFF,
	BLOCK_REDSTONE_TORCH_ON,
	BLOCK_STONE_BUTTON,
	BLOCK_SNOW,
	BLOCK_ICE,
	BLOCK_SNOW_BLOCK, /* 80 */
	BLOCK_CACTUS,
	BLOCK_CLAY_BLOCK,
	BLOCK_SUGAR_CANE,
	BLOCK_JUKEBOX,
	BLOCK_FENCE,
	BLOCK_PUMPKIN,
	BLOCK_NETHERRACK,
	BLOCK_SOUL_SAND,
	BLOCK_GLOWSTONE_BLOCK,
	BLOCK_PORTAL, /* 90 */
	BLOCK_JACKOLANTERN,
	BLOCK_CAKE,
	BLOCK_REDSTONE_REPEATER_OFF,
	BLOCK_REDSTONE_REPEATER_ON
};

enum WoolColour {
	WHITE,
	ORANGE,
	MAGENTA,
	LIGHT_BLUE,
	YELLOW,
	LIGHT_GREEN,
	PINK,
	GRAY,
	LIGHT_GRAY,
	CYAN,
	PURPLE,
	BLUE,
	BROWN,
	DARK_GREEN,
	RED,
	BLACK
};

class nbtFile;
class Tag;
class TagCompound;
/**
* @class Chunk
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
		nbtFile* chunkFile;
		ChunkData(const ChunkData&);
		ChunkData& operator=(const ChunkData&);
		friend class ChunkInterface;
		Tag* getTag(const std::string &);	
	public:
		ChunkData();
		
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

class ChunkInterface : public Counter {
	private:
		friend class RegionData;
		ChunkData* chunkPointer;
		ChunkInterface(ChunkData* pointer);
		uint8_t (*blockPointer)[CHUNKZ][CHUNKY];
		void modified();
		void setupPointers();
	public:
		ChunkInterface(const ChunkInterface&);
		void setBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos,BlockType block);	
		BlockType getBlock(uint8_t xPos,uint8_t yPos,uint8_t zPos) const;
		uint8_t getBlockData(uint8_t xPos,uint8_t yPos,uint8_t zPos) const;
};

#endif
