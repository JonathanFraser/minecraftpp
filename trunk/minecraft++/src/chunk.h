#ifndef __CHUNK_INTERFACE_H
#define __CHUNK_INTERFACE_H

#include <stdint.h>
#include "ref_count.h"
#include "nbt.h"

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

//commented out until namespaces are here

/*
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
*/
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

#endif