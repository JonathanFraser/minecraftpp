#ifndef __WORLD_H
#define __WORLD_H
#include "region.h"
#include "nbt.h"

#include <string>
#include <stdint.h>
#include <utility>
#include <vector>
#include <map>
#include <memory>

namespace minecraftpp {
class RegionData;

typedef std::pair<int32_t,int32_t> Coord; 
typedef std::vector<Coord> CoordVector;
typedef std::map<Coord,RegionData*> RegionMap;

class World : public std::enable_shared_from_this<World> {
	private:
		CoordVector coords;
		Coord topLeft;
		Coord bottomRight;
		RegionMap regions;
		std::shared_ptr<nbtFile> levelFile;
		std::string dirName;
		std::string constructFilename(int32_t x,int32_t z);
		void buildCoordList(const Coord&,CoordVector &,const std::string&);
		std::shared_ptr<nbtFile> readCompressedNBT(const std::string &filename);
		World(const std::string& );
	public:
		class iterator {
			private:
				std::shared_ptr<World> worldPointer;
				CoordVector::iterator region;
				uint8_t chunkX;
				uint8_t chunkZ;
				void addOne();
				void subOne();
			public:
				iterator(std::shared_ptr<World>);
				//set iterator to end
				iterator& end();
				//equal
				bool operator==(const iterator &) const;
				//not-equal
				bool operator!=(const iterator &) const;
				//pre-increment
				iterator & operator++();
				//post-increament
				iterator operator++(int);
				//pre-decrement 
				iterator & operator--();
				//post-decrement
				iterator operator--(int);
				//dereference
				Chunk operator*();
				//get location
				Coord getCoord();
		};
		static std::shared_ptr<World> getWorld(const std::string &worldfolder);
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
		iterator begin();
		iterator end();
};
}
#endif
