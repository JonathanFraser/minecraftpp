#include "region.h"
#include "world.h"

#include <iostream>
#include <cstdio>

BlockType maxValue(BlockType A,BlockType B) {
	return BLOCK_AIR;
}

int main(void) {
	World B("/home/jfraser/world");
	Coord tL = B.getTopLeft();
	Coord bR = B.getBottomRight();
	int32_t minX = tL.first;
	int32_t maxX = bR.first;
	int32_t minZ = bR.second;
	int32_t maxZ = tL.second;
	uint64_t dimX = (maxX-minX +1)*REGIONX*CHUNKX;
	uint64_t dimZ = (maxZ-minZ +1)*REGIONZ*CHUNKZ;
	BlockType* dataArray = new BlockType[dimX*dimZ];
	for(int l=minX;l<=maxX;l++)
		for(int m=minZ;m<=maxZ;m++)	
			if(B.regionInDir(l,m)) {
				RegionInterface A = B.getRegion(l,m);
				for(int i=0;i<REGIONZ;i++)
					for(int j=0;j<REGIONX;j++) {
						if(A.chunkInFile(j,i)) {
							const ChunkInterface &temp = A.getChunk(j,i);
							for(int z=0;z<CHUNKZ;z++)
								for(int x=0;x<CHUNKX;x++) {
									for(int y=CHUNKY-1;y>=0;y--) {
										uint64_t xloc = (l-minX)*REGIONX*CHUNKX + j*CHUNKX + x;
										uint64_t zloc = (m-minZ)*REGIONZ*CHUNKZ + i*CHUNKZ + z;
										
										uint64_t index = xloc*dimZ + zloc;
										dataArray[index] = maxValue(temp.getBlock(x,y,z),dataArray[index]);
									}
								}
						}
					}	
			}

	//do make map here


	
	delete[] dataArray;
	return 0;
}
