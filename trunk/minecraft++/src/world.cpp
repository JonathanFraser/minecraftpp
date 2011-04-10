#include "world.h"
#include "region_data.h"

#include <zlib.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

const char REGION_DIR[] = "region/";
const char FILE_PREFIX[] = "r.";
const char FILE_SUFFIX[] = ".mcr";
const char SEPARATOR[] = ".";
		
using namespace minecraftpp;

World::World(const std::string &worldfolder): topLeft(Coord(0,0)),bottomRight(Coord(0,0)),dirName(worldfolder) {
	CoordVector temp;
	if(dirName[dirName.size()] != '/') {
		dirName = dirName + '/';
	}
	buildCoordList(Coord(0,0),temp,dirName);
	levelFile = readCompressedNBT(dirName+"level.dat");
}

World::~World() {
	delete levelFile;
	for(RegionMap::iterator i=regions.begin();i!=regions.end();i++) {
		delete i->second;
	}
}

std::string World::constructFilename(int32_t x,int32_t z) {
	std::stringstream filename;
	filename << dirName;
	filename << REGION_DIR;
	filename << FILE_PREFIX;
	filename << x;
	filename << SEPARATOR;
	filename << z;
	filename << FILE_SUFFIX;
	return filename.str();	
} 

void World::buildCoordList(const Coord &test,CoordVector &tested,const std::string &dirprefix) {
	
	for(CoordVector::iterator i=tested.begin();i!=tested.end();i++) {
		if(*i == test)
			return;
	}
	
	tested.push_back(test);	
	std::ifstream ifile(constructFilename(test.first,test.second).c_str());
	if(ifile) {	
		coords.push_back(test);
		if(test.first < topLeft.first)
			topLeft = Coord(test.first,topLeft.second);
		if(test.second > topLeft.second)
			topLeft = Coord(topLeft.first,test.second);

		if(test.first > bottomRight.first)
			bottomRight =Coord(test.first,bottomRight.second);
		if(test.second < bottomRight.second)	
			bottomRight = Coord(bottomRight.first,test.second);

		buildCoordList(Coord(test.first+1,test.second),tested,dirprefix);
		buildCoordList(Coord(test.first-1,test.second),tested,dirprefix);
		buildCoordList(Coord(test.first,test.second+1),tested,dirprefix);
		buildCoordList(Coord(test.first,test.second-1),tested,dirprefix);
	}
	return;
}

nbtFile* World::readCompressedNBT(const std::string &filename) {
	gzFile gzfile = gzopen(filename.c_str(),"rb");
	assert(gzfile);
	unsigned int datalength = MEGABYTE;
	uint8_t *data = new uint8_t[datalength];
	uint8_t *holder = data;
	size_t readval = gzread(gzfile,holder,datalength);
	assert(readval != 0);
	nbtFile* temp = new nbtFile(data);
	delete[] data;
	gzclose(gzfile);
	return temp;
}

bool World::regionInDir(int32_t x,int32_t z) {
	Coord test(x,z);
	for(CoordVector::iterator i = coords.begin();i != coords.end() ;i++) {
		if(test == *i)
			return true;
	}
	return false;
}

bool World::regionLoaded(int32_t x,int32_t z) {
	Coord test(x,z);
	for(RegionMap::iterator i=regions.begin();i!=regions.end();i++) {
		if(i->first == test)
			return true;
	}
	return false;
}

Coord World::getTopLeft() {
	return topLeft;
}

Coord World::getBottomRight() {
	return bottomRight;
}

Coord World::getBottomLeft() {
	return	Coord(bottomRight.first,topLeft.second); 
}

Coord World::getTopRight() {
	return Coord(topLeft.first,bottomRight.second);
}

Region World::getRegion(int32_t x,int32_t z) {
	assert(regionInDir(x,z));
	if(!regionLoaded(x,z)) {
		regions.insert(std::pair<Coord,RegionData*>(Coord(x,z),new RegionData(constructFilename(x,z))));	
	}
	return Region(regions[Coord(x,z)]);			
}

unsigned int World::regionCount() const {
	return static_cast<unsigned int> (coords.size());
}

void World::printCoords() {
	for(CoordVector::iterator i=coords.begin();i!=coords.end();i++) {
		std::cout << "(" << i->first << "," << i->second << ")" << std::endl;
	}
}

World::iterator World::begin() {
	return iterator(this);
}

World::iterator World::end() {
	return iterator(this).end();
}

World::iterator::iterator(World* pointer) : worldPointer(pointer),region(pointer->coords.begin()),chunkX(0),chunkZ(0) {
}

World::iterator& World::iterator::end() {
	region = worldPointer->coords.end();
	chunkX=0;
	chunkZ=0;
	return *this;
}

bool World::iterator::operator==(const iterator &temp) const {
	return (temp.region == region) && (chunkX == temp.chunkX) && (chunkZ == temp.chunkZ);
}

bool World::iterator::operator!=(const iterator &temp) const {
	return !(*this == temp);
}

Chunk World::iterator::operator*() {
	return worldPointer->getRegion(region->first,region->second).getChunk(chunkX,chunkZ);
}

void World::iterator::addOne() {
	if(++chunkX == 32) {
		chunkX = 0;
		if(++chunkZ == 32) {
			chunkZ = 0;
			region++;
		}
	}
}

void World::iterator::subOne() {
	if(chunkX-- == 0) {
		chunkX = 31;
		if(chunkZ-- == 0) {
			chunkZ = 31;
			region--;
		}
	}
}

World::iterator& World::iterator::operator++() {
	if(*this == worldPointer->end())
			return *this;

	do {
		addOne();
	} while (!((worldPointer->regionInDir(region->first,region->second)) && (worldPointer->getRegion(region->first,region->second).chunkInFile(chunkX,chunkZ))) && *this != worldPointer->end() );
	return *this;
}

World::iterator& World::iterator::operator--() {
	if(*this == worldPointer->begin())
		return *this;

	do {
		subOne();
	} while (!((worldPointer->regionInDir(region->first,region->second)) && (worldPointer->getRegion(region->first,region->second).chunkInFile(chunkX,chunkZ))) && *this != worldPointer->begin() );
	return *this;
}

World::iterator World::iterator::operator++(int) {
	iterator temp = *this;
	++(*this);
	return temp;
}

World::iterator World::iterator::operator--(int) {
	iterator temp = *this;
	--(*this);
	return temp;
}

Coord World::iterator::getCoord() {
	assert(*this != worldPointer->end());
	Coord temp = *region;
	temp.first = (32*temp.first + chunkX)*16;
	temp.second = (32*temp.second + chunkZ)*16;
	return temp;
}
