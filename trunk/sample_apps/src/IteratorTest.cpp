#include "minecraftpp.h"
#include <iostream>
#include <cassert>

using namespace minecraftpp;

int main(void) {
	std::shared_ptr<World> theWorld = World::getWorld("./world");
	assert(theWorld->begin() != theWorld->end());
	for(World::iterator i=theWorld->begin();i!=theWorld->end();++i) {
		Coord temp = i.getCoord();	
		std::cout << "(" << temp.first << "," <<temp.second << ")" << std::endl;
	}
	return 0;	
}
