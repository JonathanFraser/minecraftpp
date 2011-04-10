#include "minecraftpp.h"
#include <iostream>
#include <cassert>

using namespace minecraftpp;

int main(void) {
	World theWorld("./world");
	assert(theWorld.begin() != theWorld.end());
	for(World::iterator i=theWorld.begin();i!=theWorld.end();++i) {
		Coord temp = i.getCoord();	
		std::cout << "(" << temp.first << "," <<temp.second << ")" << std::endl;
	}
	return 0;	
}
