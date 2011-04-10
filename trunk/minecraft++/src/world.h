#ifndef __WORLD_H
#define __WORLD_H
#include "region.h"
#include "nbt.h"

#include <string>
#include <stdint.h>
#include <utility>
#include <vector>
#include <map>

namespace minecraftpp {
class RegionData;

typedef std::pair<int32_t,int32_t> Coord; 
typedef std::vector<Coord> CoordVector;
typedef std::map<Coord,RegionData*> RegionMap;

class World {
	private:
		CoordVector coords;
		Coord topLeft;
		Coord bottomRight;
		RegionMap regions;
		nbtFile* levelFile;
		std::string dirName;
		std::string constructFilename(int32_t x,int32_t z);
		void buildCoordList(const Coord&,CoordVector &,const std::string&);
		nbtFile* readCompressedNBT(const std::string &filename);
	public:
		World(const std::string& );
		~World();
		void printCoords();
		bool regionInDir(int32_t x,int32_t z);
		bool regionLoaded(int32_t x,int32_t z);
		Coord getTopLeft();
		Coord getTopRight();
		Coord getBottomLeft();
		Coord getBottomRight();
		Region getRegion(int32_t x,int32_t z);
		unsigned int regionCount() const;
};
}
#endif
