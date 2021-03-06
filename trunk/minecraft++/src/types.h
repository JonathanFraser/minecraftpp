#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>
#include <string>

namespace minecraftpp {

const uint32_t MEGABYTE = 1024*1024;
const uint16_t SECTOR_SIZE = 4096;

const uint8_t CHUNKX = 16;
const uint8_t CHUNKY = 128;
const uint8_t CHUNKZ = 16;

const uint8_t REGIONX = 32;
const uint8_t REGIONZ = 32;

//Chunk File String Constants
const std::string LEVEL = "Level";
const std::string BLOCKS = "Blocks";
const std::string DATA = "Data";
const std::string SKY_LIGHT = "SkyLight";
const std::string LIGHT = "BlockLight";
const std::string HEIGHT_MAP = "HeightMap";
const std::string ENTITIES = "Entities";
const std::string TILE_ENTITIES = "TileEntities";
const std::string LAST_UPDATE ="LastUpdate";
const std::string X_POS = "xPos";
const std::string Z_POS = "zPos";
const std::string TERRAIN_POPULATED = "TerrainPopulated";


enum class BlockType : uint8_t {
	AIR, /* 0 */
	STONE,
	GRASS,
	DIRT,
	COBBLESTONE,
	PLANK,
	SAPLING,
	BEDROCK,
	WATER,
	STATIONARY_WATER,
	LAVA, /* 10 */
	STATIONARY_LAVA,
	SAND,
	GRAVEL,
	GOLD_ORE,
	IRON_ORE,
	COAL_ORE,
	WOOD,
	LEAVES,
	SPONGE,
	GLASS, /* 20 */
	LAPIS_ORE,
	LAPIS_BLOCK,
	DISPENSER,
	SANDSTONE,
	NOTE_BLOCK,
	BED,
	POWERED_RAIL,
	DETECTOR_RAIL,
	STICKY_PISTON,
	COBWEB, /* 30 */
	TALL_GRASS,
	DEAD_BRUSH,
	PISTON,
	PISTON_EXTENSION,
	WOOL,
	BLOCK_MOVED_BY_PISTON,
	YELLOW_FLOWER,
	ROSE,
	BROWN_MUSHROOM,
	RED_MUSHROOM, /* 40 */
	GOLD_BLOCK,
	IRON_BLOCK,
	DOUBLE_SLAB,
	SLAB,
	BRICK_BLOCK,
	TNT,
	BOOKSHELF,
	MOSSY_COBBLESTONE,
	OBSIDIAN,
	TORCH, /* 50 */
	FIRE,
	SPAWNER,
	WOODEN_STAIRS,
	CHEST,
	REDSTONE_WIRE,
	DIAMOND_ORE,
	DIAMOND_BLOCK,
	CRAFTING_TABLE,
	CROPS,
	FARMLAND, /* 60 */
	FURNACE,
	BURNING_FURNACE,
	SIGN_POST,
	WOODEN_DOOR,
	LADDER,
	RAILS,
	COBBLESTONE_STAIRS,
	WALL_SIGN,
	LEVER,
	STONE_PRESSURE_PLATE, /* 70 */
	IRON_DOOR,
	WOODEN_PRESSURE_PLATE,
	REDSTONE_ORE,
	GLOWING_REDSTONE_ORE,
	REDSTONE_TORCH_OFF,
	REDSTONE_TORCH_ON,
	STONE_BUTTON,
	SNOW,
	ICE,
	SNOW_BLOCK, /* 80 */
	CACTUS,
	CLAY_BLOCK,
	SUGAR_CANE,
	JUKEBOX,
	FENCE,
	PUMPKIN,
	NETHERRACK,
	SOUL_SAND,
	GLOWSTONE_BLOCK,
	PORTAL, /* 90 */
	JACKOLANTERN,
	CAKE,
	REDSTONE_REPEATER_OFF,
	REDSTONE_REPEATER_ON,
	LOCKED_CHEST,
	TRAP_DOOR,
	HIDDEN_SILVER_FISH,
	STONE_BRICKS,
	HUGE_BROWN_MUSHROOM,
	HUGE_RED_MUSHROOM, /* 100 */
	IRON_BARS,
	GLASS_PANE,
	MELON,
	PUMPKIN_STEM,
	MELON_STEM,
	VINES,
	FENCE_GATE,
	BRICK_STAIRS,
	STONE_BRICK_STAIRS,
	MYCELIUM, /* 110 */
	LILY_PAD,
	NETHER_BRICK,
	NETHER_BRICK_FENCE,
	NETHER_BRICK_STAIRS,
	NETHER_WART,
	ENCHANTMENT_TABLE,
	BREWING_STAND,
	CAULDRON,
	AIR_PORTAL,
	AIR_PORTAL_FRAME /* 120 */
};

enum class WoolColour : uint8_t {
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

}
#endif
