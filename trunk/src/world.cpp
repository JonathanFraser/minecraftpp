#include "world.h"
#include <zlib.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

const std::string World::REGION_DIR = "/region/";
const std::string World::FILE_PREFIX = "r.";
const std::string World::FILE_SUFFIX = ".mcr";
const std::string World::SEPARATOR = ".";
		

World::World(const std::string &worldfolder): topLeft(Coord(0,0)),bottomRight(Coord(0,0)),dirName(worldfolder) {
	CoordVector temp;
	buildCoordList(Coord(0,0),temp,worldfolder);
	std::ifstream levelfile((worldfolder+"/level.dat").c_str());
	gzFile gzfile = gzopen((worldfolder+"/level.dat").c_str(),"rb");
	size_t datalength = MEGABYTE;
	uint8_t *data = new uint8_t[datalength];
	size_t readval = gzread(gzfile,data,datalength);
	assert(readval != 0);
	levelFile = new nbtFile(data);
	delete[] data;
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

RegionInterface World::getRegion(int32_t x,int32_t z) {
	assert(regionInDir(x,z));
	if(!regionLoaded(x,z)) {
		regions.insert(std::pair<Coord,RegionData*>(Coord(x,z),new RegionData(constructFilename(x,z))));	
	}
	return RegionInterface(regions[Coord(x,z)]);			
}

unsigned int World::regionCount() const {
	return coords.size();
}

void World::printCoords() {
	for(CoordVector::iterator i=coords.begin();i!=coords.end();i++) {
		std::cout << "(" << i->first << "," << i->second << ")" << std::endl;
	}
}
