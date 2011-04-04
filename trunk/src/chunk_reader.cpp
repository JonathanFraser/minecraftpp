#include "region_interface.h"
#include "world.h"

#include <cassert>
#include <iostream>
#include <cstdio>
#define greater(A,B,C) if(A == C || B == C) return C

BlockType maxValue(BlockType curr,BlockType prev) {
	greater(curr,prev,BLOCK_CLAY_BLOCK);
	if(curr != BLOCK_AIR && prev == BLOCK_AIR) {
			if(curr == BLOCK_WATER || curr == BLOCK_STATIONARY_WATER || curr == BLOCK_ICE)
				return BLOCK_WATER;
			return BLOCK_DIRT;
	}
	return prev;
}

int main(void) {
	World B("./world");
	Coord tL = B.getTopLeft();
	Coord bR = B.getBottomRight();
	BlockType* dataArray = new BlockType[16*16*32*32];
	if(B.regionInDir(0,-2)) {
		const RegionInterface &A = B.getRegion(0,-2);
		for(int chunkZ=0;chunkZ<REGIONZ;chunkZ++)
			for(int chunkX=0;chunkX<REGIONX;chunkX++) {
				if(A.chunkInFile(chunkX,chunkZ)) {
					const ChunkInterface &temp = A.getChunk(chunkX,chunkZ);
					for(int z=0;z<CHUNKZ;z++)
						for(int x=0;x<CHUNKX;x++) {
							uint64_t xloc = chunkX*16 + x;
							uint64_t zloc = chunkZ*16 + z;
							uint64_t index = xloc*16*32 + (16*32 -1 - zloc);
							dataArray[index]= BLOCK_AIR;
							for(uint8_t y=CHUNKY-1;y<128;y--) {
								BlockType temp2 = maxValue(temp.getBlock(x,y,z),dataArray[index]);
								assert(!(temp2 == BLOCK_AIR && dataArray[index] != BLOCK_AIR));
								dataArray[index] = temp2;
							}
						}
					}
				}	
	}

	for(uint64_t i = 0;i<16*32;i++) {
		for(uint64_t j = 0;j<16*32;j++) {
			//std::cout << dataArray[i*16*32 + j] << ",";	
		}
		//std::cout << std::endl;
	}
	//do make map here


	
	delete[] dataArray;
	return 0;
}
