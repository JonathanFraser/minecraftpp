#include "region_interface.h"
#include "world.h"

#include <pqxx/pqxx>
 
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <map>
#include <vector>


struct location {
	int64_t x;
	int64_t z;
	uint8_t y;
};

const uint8_t trackedblocks[] = {
	BLOCK_GOLD_ORE,
	BLOCK_IRON_ORE,
	BLOCK_COAL_ORE,
	BLOCK_LAPIS_ORE,
	BLOCK_YELLOW_FLOWER,
	BLOCK_ROSE,
	BLOCK_BROWN_MUSHROOM,
	BLOCK_RED_MUSHROOM, /* 40 */
	BLOCK_MOSSY_COBBLESTONE,
	BLOCK_OBSIDIAN,
	BLOCK_SPAWNER,
	BLOCK_DIAMOND_ORE,
	BLOCK_REDSTONE_ORE,
	BLOCK_GLOWING_REDSTONE_ORE,
	BLOCK_CLAY_BLOCK,
	BLOCK_PUMPKIN
};

typedef std::vector<location> LocVector;
typedef std::map<uint8_t,LocVector> CoordMap;
typedef std::pair<uint8_t,LocVector> MapItem;


int main(void) {
	World B("./world");
	CoordMap points;
	Coord tR = B.getTopRight();
	Coord bL = B.getBottomLeft();
	std::map<uint8_t,uint64_t> counts;
	
	for(uint8_t i=0;i<sizeof(trackedblocks);i++) {
		points.insert(MapItem(trackedblocks[i],LocVector()));
		counts.insert(std::pair<uint8_t,uint64_t>(trackedblocks[i],0));
	}

	for(int64_t regionX=tR.first;regionX<bL.first;regionX++)
		for(int64_t regionZ=tR.second;regionZ<bL.second;regionZ++) {
			if(B.regionInDir(regionX,regionZ)) {
				const RegionInterface &A = B.getRegion(regionX,regionZ);
				for(int chunkZ=0;chunkZ<REGIONZ;chunkZ++)
					for(int chunkX=0;chunkX<REGIONX;chunkX++) {
						if(A.chunkInFile(chunkX,chunkZ)) {
						const ChunkInterface &temp = A.getChunk(chunkX,chunkZ);
						for(int z=0;z<CHUNKZ;z++)
							for(int x=0;x<CHUNKX;x++) {
								int64_t xloc = regionX*32*16 + chunkX*16 + x;
								int64_t zloc = regionZ*32*16 + chunkZ*16 + z;
								for(uint8_t y=0;y<128;y++) {
									uint8_t block = temp.getBlock(x,y,z);
									location loc;
									loc.x = xloc;
									loc.y = y;
									loc.z = zloc;
									CoordMap::iterator iter = points.find(block);
									if( iter != points.end()) {
											iter->second.push_back(loc);	
											counts.find(block)->second++;
									}
								}	
							}
						}
					}
					
			}
		}
	
	
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
		
	return 0;
}
