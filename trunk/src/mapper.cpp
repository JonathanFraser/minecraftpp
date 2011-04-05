#include "world.h"
#include <png++/png.hpp>
#include <sstream>
#include <iostream>

unsigned long int array[256];

typedef png::image<png::rgb_pixel> PNG;

void scanChunk(PNG* image,unsigned int xoffset,unsigned int zoffset, ChunkInterface chunk) {
	for(unsigned int x=0;x<16;x++)
		for(unsigned int z=0;z<16;z++)
			for(unsigned int y=0;y<128;y++) {
				array[chunk.getBlock(x,y,z)]++;
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
	uint8_t ID;
	for(int i=0;i<256;i++)
		array[i]=0;

	std::string worlddir(argv[1]);
	std::string blockid(argv[2]);
	std::stringstream stream;
	stream.str(blockid);
	stream >> ID;
	World map(worlddir);
	Coord tL = map.getTopLeft();
	Coord bR = map.getBottomRight();
	unsigned int width = tL.second - bR.second+1; 
	unsigned int height = bR.first - tL.first+1;  
	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;
	PNG image(512*height,512*width);
	for(long int x = tL.first;x<=bR.first;x++)
		for(long int z = bR.second;z<=tL.second;z++){
		if(map.regionInDir(x,z)) {
			scanRegion(&image,(x-tL.first)*512,(tL.second - z)*512,map.getRegion(x,z));
		}
	}
	
	for(int i=0;i<256;i++) {
	//	std::cout << i << " : " << array[i] << std::endl;
	}
	return 0;
}
