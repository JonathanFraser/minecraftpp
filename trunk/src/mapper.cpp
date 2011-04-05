#include "world.h"
#include <png++/png.hpp>
#include <sstream>
#include <iostream>

uint8_t ID;
typedef png::rgb_pixel pixel;
typedef png::image<pixel> PNG;

pixel getColour(BlockType block) {
	if(block == ID)
		return pixel(255,0,0);

	if(block == BLOCK_DIRT)
		return pixel(0,255,0);
	
	if(block == BLOCK_WATER)
		return pixel(0,0,255);
	
	assert(false);
	return pixel(0,0,0);
}	

BlockType getMaximum(BlockType old_block,BlockType new_block) {
	if(new_block == ID || old_block == ID)
		return static_cast<BlockType>(ID);
	
	if(new_block != BLOCK_AIR && old_block == BLOCK_AIR) {

		if(new_block == BLOCK_WATER || new_block == BLOCK_STATIONARY_WATER || new_block == BLOCK_ICE)
			return BLOCK_WATER;

		return BLOCK_DIRT;
	}

	return old_block; 
}

void scanChunk(PNG* image,unsigned int xoffset,unsigned int zoffset, ChunkInterface chunk) {
	for(unsigned int x=0;x<16;x++)
		for(unsigned int z=0;z<16;z++) {
			unsigned int xvar = xoffset + x;
			unsigned int zvar = zoffset + (15-z);
			BlockType temp = BLOCK_AIR;
			for(unsigned int y=127;y<128;y--) {
				temp = getMaximum(temp,chunk.getBlock(x,y,z));
			}
			(*image)[xvar][zvar] = getColour(temp);
		}
	return;
}

void scanRegion(PNG* image,unsigned int xoffset,unsigned int zoffset, RegionInterface region) {
	for(unsigned int x=0;x<32;x++) {
		for(unsigned int z=0;z<32;z++) {
			if(region.chunkInFile(x,z)) {
				scanChunk(image,xoffset+x*16,zoffset + (31-z)*16,region.getChunk(x,z));	
			}
		}
	}
	return;
}

std::ostream & operator<<(std::ostream & stream,Coord &x) {
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
	World map(worlddir);
	Coord tL = map.getTopLeft();
	Coord bR = map.getBottomRight();
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
		} else {
			for(unsigned int i=0;i<512;i++)
				for(unsigned int j=0;j<512;j++) {
					assert(xoffset+i < image.get_height());
					assert(zoffset+j < image.get_width());
					image[xoffset+i][zoffset+j] = pixel(255,255,255);
				}
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
