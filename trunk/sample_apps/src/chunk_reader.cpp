#include "minecraftpp.h"

#include <pqxx/pqxx>
 
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

using namespace minecraftpp;

struct location {
	int64_t x;
	int64_t z;
	uint8_t y;
};

const BlockType trackedblocks[] = {
	BlockType::GOLD_ORE,
	BlockType::IRON_ORE,
	BlockType::COAL_ORE,
	BlockType::LAPIS_ORE,
	BlockType::YELLOW_FLOWER,
	BlockType::ROSE,
	BlockType::BROWN_MUSHROOM,
	BlockType::RED_MUSHROOM, /* 40 */
	BlockType::MOSSY_COBBLESTONE,
	BlockType::OBSIDIAN,
	BlockType::SPAWNER,
	BlockType::DIAMOND_ORE,
	BlockType::REDSTONE_ORE,
	BlockType::GLOWING_REDSTONE_ORE,
	BlockType::CLAY_BLOCK,
	BlockType::PUMPKIN
};

const uint32_t ABOVE = 1<<0;
const uint32_t BELOW = 1<<1;
const uint32_t WEST = 1<<2;
const uint32_t EAST = 1<<3;
const uint32_t NORTH = 1<<4;
const uint32_t SOUTH = 1<<5;
const uint32_t NORTH_WEST = 1<<6;
const uint32_t NORTH_EAST = 1<<7;
const uint32_t SOUTH_WEST = 1<<8;
const uint32_t SOUTH_EAST = 1<<9;
const uint32_t ABOVE_WEST = 1<<10;
const uint32_t ABOVE_EAST = 1<<11;
const uint32_t ABOVE_NORTH = 1<<12;
const uint32_t ABOVE_SOUTH = 1<<13;
const uint32_t ABOVE_NORTH_WEST = 1<<14;
const uint32_t ABOVE_NORTH_EAST = 1<<15;
const uint32_t ABOVE_SOUTH_WEST = 1<<16;
const uint32_t ABOVE_SOUTH_EAST = 1<<17;
const uint32_t BELOW_WEST = 1<<18;
const uint32_t BELOW_EAST = 1<<19;
const uint32_t BELOW_NORTH = 1<<20;
const uint32_t BELOW_SOUTH = 1<<21;
const uint32_t BELOW_NORTH_WEST = 1<<22;
const uint32_t BELOW_NORTH_EAST = 1<<23;
const uint32_t BELOW_SOUTH_WEST = 1<<24;
const uint32_t BELOW_SOUTH_EAST = 1<<25;





class block {
	public:
		int32_t x;
		int32_t z;
		uint8_t y;
		uint32_t facesChecked;
		uint8_t openFaces;
		block(uint8_t blockid , int32_t x,int32_t z,uint8_t y) :x(x),z(z),y(y),facesChecked(0),openFaces(26) {};
};

class blob {
	public:
		std::vector<block> exposed_blocks;
		int32_t x;
		int32_t z;
		uint8_t y;
		uint16_t count;
};

bool tracked[256];

typedef std::vector<blob> BlobVector;

int main(void) {
	std::shared_ptr<World> B = World::getWorld("./world");
	Coord tR = B->getTopRight();
	Coord bL = B->getBottomLeft();
	BlobVector blobs; 
	
	memset(tracked,0,sizeof(tracked));
	for(uint8_t i=0;i<sizeof(trackedblocks);i++) {
		tracked[static_cast<uint8_t>(trackedblocks[i])]=true;
	}

	for(int32_t regionX=tR.first;regionX<bL.first;regionX++)
		for(int32_t regionZ=tR.second;regionZ<bL.second;regionZ++) {
			if(B->regionInDir(regionX,regionZ)) {
				const Region &A = B->getRegion(regionX,regionZ);
				for(uint8_t chunkZ=0;chunkZ<REGIONZ;chunkZ++)
					for(uint8_t chunkX=0;chunkX<REGIONX;chunkX++) {
						if(A.chunkInFile(chunkX,chunkZ)) {
						const Chunk &temp = A.getChunk(chunkX,chunkZ);
						bool visited[16][128][16];
						memset(visited,0,sizeof(visited));
						for(uint8_t z=0;z<CHUNKZ;z++)
							for(uint8_t x=0;x<CHUNKX;x++) {
								int32_t xloc = regionX*32*16 + chunkX*16 + x;
								int32_t zloc = regionZ*32*16 + chunkZ*16 + z;
								for(uint8_t y=0;y<128;y++) {
									if(!visited[x][y][z]) {
										visited[x][y][z] = true;
										uint8_t block = static_cast<uint8_t>(temp.getBlock(x,y,z));
										if(tracked[block]) {											
										}
									}
								}	
							}
						}
					}
					
			}
		}
	
	/*
	pqxx::connection Conn("dbname=minecraft");	
	std::vector<std::string> names;
	names.push_back("id");
	names.push_back("x");
	names.push_back("z");
	names.push_back("y");
	for(CoordMap::iterator i= points.begin();i!=points.end();i++) {
		pqxx::work trans(Conn);
		pqxx::tablewriter table(trans,"resources",names.begin(),names.end());
		for(LocVector::iterator j = i->second.begin();j<i->second.end();j++) {
		std::stringstream stream;
		std::vector<long int> tuple;
			tuple.push_back(i->first);
			tuple.push_back(j->x);
			tuple.push_back(j->z);
			tuple.push_back(j->y);

		//std::cout << table.generate(tuple) << std::endl;
		table.push_back(tuple);
		}
		table.complete();
		trans.commit();
	}
		*/
	return 0;
}
