#include "minecraftpp.h"
#include <png++/png.hpp>
#include <sstream>
#include <iostream>

using namespace minecraftpp;

BlockType ID;

typedef png::rgb_pixel pixel;
typedef png::image<pixel> PNG;

const pixel BLUE = pixel(0,0,255);
const pixel RED = pixel(255,0,0);
const pixel GREEN = pixel(0,255,0);
const pixel WHITE = pixel(255,255,255);
const pixel BLACK = pixel(0,0,0);
const pixel ICEBERG = pixel(113,166,210);
const pixel CADET_GRAY = pixel(145,163,176);
const pixel KELLY_GREEN = pixel(76,187,23);
const pixel RUSSET = pixel(128,70,27);
const pixel FLAME = pixel(226,88,34);
const pixel EGYPTIAN_BLUE = pixel(16,52,166);
const pixel BUFF = pixel(240,220,130);
const pixel TYRIAN_PURPLE = pixel(102,2,60);
const pixel FOREST_GREEN = pixel(34,139,34);
const pixel CHARCOAL = pixel(54,69,79);
const pixel BRONZE = pixel(205,127,50);
const pixel GHOST_WHITE = pixel(248,248,255);
const pixel CANDY_APPLE_RED = pixel(255,8,0);
const pixel OFFICE_GREEN = pixel(0,128,0);


pixel getColour(BlockType block) {	
	if(block == ID)
		return pixel(255,0,0);
	
switch(block) {
	
	//Stoney stuff
	case BlockType::STONE:
	case BlockType::COBBLESTONE:
	case BlockType::COBBLESTONE_STAIRS:
	case BlockType::DISPENSER:
	case BlockType::DOUBLE_SLAB:
	case BlockType::SLAB:
	case BlockType::FURNACE:
	case BlockType::BURNING_FURNACE:
	case BlockType::STONE_PRESSURE_PLATE:
	case BlockType::CLAY_BLOCK:
		return CADET_GRAY;
	
	//Grass
	case BlockType::GRASS:
		return KELLY_GREEN;

	//Dirt	
	case BlockType::DIRT:
	case BlockType::FARMLAND:
		return RUSSET;

	//Water
	case BlockType::WATER:
	case BlockType::STATIONARY_WATER:
		return EGYPTIAN_BLUE; 

	//Lava and brimstone
	case BlockType::LAVA:
	case BlockType::STATIONARY_LAVA:
	case BlockType::FIRE:
		return FLAME;

	//Sandy stuff
	case BlockType::SANDSTONE:
	case BlockType::SAND:
		return BUFF;

	//gravels
	case BlockType::GRAVEL:
		return TYRIAN_PURPLE;	
		

	//Ores
	case BlockType::GOLD_ORE:
	case BlockType::IRON_ORE:
	case BlockType::COAL_ORE:
	case BlockType::LAPIS_ORE:
	case BlockType::DIAMOND_ORE:
	case BlockType::REDSTONE_ORE:
	case BlockType::GLOWING_REDSTONE_ORE:
		return CHARCOAL;
		
	
	case BlockType::CRAFTING_TABLE:
	case BlockType::PLANK:
	case BlockType::WOODEN_STAIRS:
	case BlockType::WOODEN_DOOR:
	case BlockType::SIGN_POST:
	case BlockType::LADDER:
	case BlockType::NOTE_BLOCK:
	case BlockType::CHEST:
	case BlockType::WOODEN_PRESSURE_PLATE:
	case BlockType::JUKEBOX:
	case BlockType::FENCE:
	case BlockType::WALL_SIGN:
		return BRONZE;
	
	
	//Forestry
	case BlockType::SAPLING:
	case BlockType::WOOD:
	case BlockType::LEAVES:
		return FOREST_GREEN;


	//Misc Manmade stuff
	case BlockType::GLASS:
	case BlockType::LAPIS_BLOCK:
	case BlockType::GOLD_BLOCK:
	case BlockType::IRON_BLOCK:
	case BlockType::BRICK_BLOCK:
	case BlockType::DIAMOND_BLOCK:
	case BlockType::TNT:
	case BlockType::BOOKSHELF:
		return BLACK;
	
	//The soft and fluffy
	case BlockType::BED:
	case BlockType::WOOL:
		return WHITE;
	
	//Gardeners Delight
	case BlockType::CACTUS:
	case BlockType::CROPS:
	case BlockType::SUGAR_CANE:
	case BlockType::YELLOW_FLOWER:
	case BlockType::ROSE:
	case BlockType::BROWN_MUSHROOM:
	case BlockType::RED_MUSHROOM:
		return OFFICE_GREEN;
	
	//Redstone stuff
	case BlockType::REDSTONE_WIRE:
	case BlockType::REDSTONE_TORCH_OFF:
	case BlockType::REDSTONE_TORCH_ON:
	case BlockType::REDSTONE_REPEATER_OFF:
	case BlockType::REDSTONE_REPEATER_ON:
		return CANDY_APPLE_RED;	

	//Snowy Stuff	
	case BlockType::SNOW:
	case BlockType::SNOW_BLOCK:
		return GHOST_WHITE;
	
	
	//Ice
	case BlockType::ICE:
		return ICEBERG;	

	//MISC Group
	case BlockType::SPONGE:
	case BlockType::BEDROCK:
	case BlockType::MOSSY_COBBLESTONE:
	case BlockType::OBSIDIAN:
	case BlockType::TORCH: 
	case BlockType::SPAWNER:
	case BlockType::RAILS:
	case BlockType::LEVER:
	case BlockType::IRON_DOOR:
	case BlockType::STONE_BUTTON:
	case BlockType::PUMPKIN:
	case BlockType::NETHERRACK:
	case BlockType::SOUL_SAND:
	case BlockType::GLOWSTONE_BLOCK:
	case BlockType::PORTAL:
	case BlockType::JACKOLANTERN:
	case BlockType::CAKE:
	default:
		return BLACK;
}
}	

minecraftpp::BlockType getMaximum(minecraftpp::BlockType old_block,minecraftpp::BlockType new_block) {

	if(new_block == ID || old_block == ID)
		return static_cast<minecraftpp::BlockType>(ID);
	
	if(new_block == BlockType::SNOW && old_block == BlockType::LEAVES)
		return old_block;
		
	if((new_block == BlockType::AIR || new_block == BlockType::GLASS) && old_block != BlockType::AIR) {

		return old_block;
	}

	return new_block; 
}

void scanChunk(PNG* image,unsigned int xoffset,unsigned int zoffset, minecraftpp::ChunkInterface chunk) {
	for(uint8_t x=0;x<16;x++)
		for(uint8_t z=0;z<16;z++) {
			unsigned int xvar = xoffset + x;
			unsigned int zvar = zoffset + (15-z);
			BlockType temp = BlockType::BEDROCK;
			for(uint8_t y=0;y<128;y++) {
				temp = getMaximum(temp,chunk.getBlock(x,y,z));
			}
			(*image)[xvar][zvar] = getColour(temp);
		}
	return;
}

void scanRegion(PNG* image,unsigned int xoffset,unsigned int zoffset, minecraftpp::RegionInterface region) {
	for(uint8_t x=0;x<32;x++) {
		for(uint8_t z=0;z<32;z++) {
			if(region.chunkInFile(x,z)) {
				scanChunk(image,xoffset+x*16,zoffset + (31-z)*16,region.getChunk(x,z));	
			}
		}
	}
	return;
}

std::ostream & operator<<(std::ostream & stream,minecraftpp::Coord &x) {
	stream << "(" << x.first << "," << x.second << ")";
	return stream;
}

int main(int argc,char *argv[]) {
	if(argc != 3) {
		std::cout << "Correct use is: \n \t mapper world_dir blockID" << std::endl;
		return -1;
	}
	uint32_t dataval;
	std::string worlddir(argv[1]);
	std::string blockid(argv[2]);
	std::stringstream stream;
	stream.str(blockid);
	stream >> dataval;
	ID = static_cast<BlockType>(dataval);
	World map(worlddir);
	Coord tL = map.getTopLeft();
	Coord bR = map.getBottomRight();
	unsigned int width = tL.second - bR.second+1; 
	unsigned int height = bR.first - tL.first+1;  
//	std::cout << "Width: " << width << std::endl;
//	std::cout << "Height: " << height << std::endl;
	PNG image(512*width,512*height);
	for(int32_t x = tL.first;x<=bR.first;x++)
		for(int32_t z = bR.second;z<=tL.second;z++){
			int32_t xoffset = (x-tL.first)*512;
			int32_t zoffset = (tL.second - z)*512;
			if(map.regionInDir(x,z)) {
				scanRegion(&image,xoffset,zoffset,map.getRegion(x,z));
			}
		}
	std::stringstream png_name;
	png_name << "BLOCKID_";
	png_name << static_cast<int>(ID);
	png_name << ".png";
	std::cout << png_name.str() << std::endl;
	image.write(png_name.str().c_str());	
	return 0;
}
