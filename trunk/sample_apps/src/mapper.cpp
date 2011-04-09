#include "world.h"
#include <png++/png.hpp>
#include <sstream>
#include <iostream>


uint8_t ID;
typedef png::rgb_pixel pixel;
typedef png::image<pixel> PNG;
namespace colours {
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
}

pixel getColour(minecraftpp::BlockType block) {
	using namespace minecraftpp;	
	if(block == ID)
		return pixel(255,0,0);
	
switch(block) {
	
	//Stoney stuff
	case BLOCK_STONE:
	case BLOCK_COBBLESTONE:
	case BLOCK_COBBLESTONE_STAIRS:
	case BLOCK_DISPENSER:
	case BLOCK_DOUBLE_SLAB:
	case BLOCK_SLAB:
	case BLOCK_FURNACE:
	case BLOCK_BURNING_FURNACE:
	case BLOCK_STONE_PRESSURE_PLATE:
	case BLOCK_CLAY_BLOCK:
		return colours::CADET_GRAY;
	
	//Grass
	case BLOCK_GRASS:
		return colours::KELLY_GREEN;

	//Dirt	
	case BLOCK_DIRT:
	case BLOCK_FARMLAND:
		return colours::RUSSET;

	//Water
	case BLOCK_WATER:
	case BLOCK_STATIONARY_WATER:
		return colours::EGYPTIAN_BLUE; 

	//Lava and brimstone
	case BLOCK_LAVA:
	case BLOCK_STATIONARY_LAVA:
	case BLOCK_FIRE:
		return colours::FLAME;

	//Sandy stuff
	case BLOCK_SANDSTONE:
	case BLOCK_SAND:
		return colours::BUFF;

	//gravels
	case BLOCK_GRAVEL:
		return colours::TYRIAN_PURPLE;	
		

	//Ores
	case BLOCK_GOLD_ORE:
	case BLOCK_IRON_ORE:
	case BLOCK_COAL_ORE:
	case BLOCK_LAPIS_ORE:
	case BLOCK_DIAMOND_ORE:
	case BLOCK_REDSTONE_ORE:
	case BLOCK_GLOWING_REDSTONE_ORE:
		return colours::CHARCOAL;
		
	
	case BLOCK_CRAFTING_TABLE:
	case BLOCK_PLANK:
	case BLOCK_WOODEN_STAIRS:
	case BLOCK_WOODEN_DOOR:
	case BLOCK_SIGN_POST:
	case BLOCK_LADDER:
	case BLOCK_NOTE_BLOCK:
	case BLOCK_CHEST:
	case BLOCK_WOODEN_PRESSURE_PLATE:
	case BLOCK_JUKEBOX:
	case BLOCK_FENCE:
	case BLOCK_WALL_SIGN:
		return colours::BRONZE;
	
	
	//Forestry
	case BLOCK_SAPLING:
	case BLOCK_WOOD:
	case BLOCK_LEAVES:
		return colours::FOREST_GREEN;


	//Misc Manmade stuff
	case BLOCK_GLASS:
	case BLOCK_LAPIS_BLOCK:
	case BLOCK_GOLD_BLOCK:
	case BLOCK_IRON_BLOCK:
	case BLOCK_BRICK_BLOCK:
	case BLOCK_DIAMOND_BLOCK:
	case BLOCK_TNT:
	case BLOCK_BOOKSHELF:
		return colours::BLACK;
	
	//The soft and fluffy
	case BLOCK_BED:
	case BLOCK_WOOL:
		return colours::WHITE;
	
	//Gardeners Delight
	case BLOCK_CACTUS:
	case BLOCK_CROPS:
	case BLOCK_SUGAR_CANE:
	case BLOCK_YELLOW_FLOWER:
	case BLOCK_ROSE:
	case BLOCK_BROWN_MUSHROOM:
	case BLOCK_RED_MUSHROOM:
		return colours::OFFICE_GREEN;
	
	//Redstone stuff
	case BLOCK_REDSTONE_WIRE:
	case BLOCK_REDSTONE_TORCH_OFF:
	case BLOCK_REDSTONE_TORCH_ON:
	case BLOCK_REDSTONE_REPEATER_OFF:
	case BLOCK_REDSTONE_REPEATER_ON:
		return colours::CANDY_APPLE_RED;	

	//Snowy Stuff	
	case BLOCK_SNOW:
	case BLOCK_SNOW_BLOCK:
		return colours::GHOST_WHITE;
	
	
	//Ice
	case BLOCK_ICE:
		return colours::ICEBERG;	

	//MISC Group
	case BLOCK_SPONGE:
	case BLOCK_BEDROCK:
	case BLOCK_MOSSY_COBBLESTONE:
	case BLOCK_OBSIDIAN:
	case BLOCK_TORCH: 
	case BLOCK_SPAWNER:
	case BLOCK_RAILS:
	case BLOCK_LEVER:
	case BLOCK_IRON_DOOR:
	case BLOCK_STONE_BUTTON:
	case BLOCK_PUMPKIN:
	case BLOCK_NETHERRACK:
	case BLOCK_SOUL_SAND:
	case BLOCK_GLOWSTONE_BLOCK:
	case BLOCK_PORTAL:
	case BLOCK_JACKOLANTERN:
	case BLOCK_CAKE:
	default:
		return colours::BLACK;
}
}	

minecraftpp::BlockType getMaximum(minecraftpp::BlockType old_block,minecraftpp::BlockType new_block) {
	using namespace minecraftpp;
	if(new_block == ID || old_block == ID)
		return static_cast<minecraftpp::BlockType>(ID);
	
	if(new_block == BLOCK_SNOW && old_block == BLOCK_LEAVES)
		return old_block;
		
	if((new_block == BLOCK_AIR || new_block == BLOCK_GLASS) && old_block != BLOCK_AIR) {

		return old_block;
	}

	return new_block; 
}

void scanChunk(PNG* image,unsigned int xoffset,unsigned int zoffset, minecraftpp::ChunkInterface chunk) {
	for(unsigned int x=0;x<16;x++)
		for(unsigned int z=0;z<16;z++) {
			unsigned int xvar = xoffset + x;
			unsigned int zvar = zoffset + (15-z);
			minecraftpp::BlockType temp = minecraftpp::BLOCK_BEDROCK;
			for(unsigned int y=0;y<128;y++) {
				temp = getMaximum(temp,chunk.getBlock(x,y,z));
			}
			(*image)[xvar][zvar] = getColour(temp);
		}
	return;
}

void scanRegion(PNG* image,unsigned int xoffset,unsigned int zoffset, minecraftpp::RegionInterface region) {
	for(unsigned int x=0;x<32;x++) {
		for(unsigned int z=0;z<32;z++) {
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
	ID = dataval;
	minecraftpp::World map(worlddir);
	minecraftpp::Coord tL = map.getTopLeft();
	minecraftpp::Coord bR = map.getBottomRight();
	unsigned int width = tL.second - bR.second+1; 
	unsigned int height = bR.first - tL.first+1;  
//	std::cout << "Width: " << width << std::endl;
//	std::cout << "Height: " << height << std::endl;
	PNG image(512*width,512*height);
	for(long int x = tL.first;x<=bR.first;x++)
		for(long int z = bR.second;z<=tL.second;z++){
			unsigned int xoffset = (x-tL.first)*512;
			unsigned int zoffset = (tL.second - z)*512;
			if(map.regionInDir(x,z)) {
				scanRegion(&image,xoffset,zoffset,map.getRegion(x,z));
			}
		}
	std::stringstream png_name;
	png_name << "BLOCKID_";
	png_name << (int)ID;
	png_name << ".png";
	std::cout << png_name.str() << std::endl;
	image.write(png_name.str().c_str());	
	return 0;
}
