#include "world.h"
#include <iostream>

int main(int argc,char *argv[]) {
	if(argc != 3) {
		std::cout << "Correct use is: \n \t mapper world_dir blockID" << std::endl;
		return -1;
	}
	std::string worlddir(argv[1]);
	World map(worlddir);
	Coord tL = map.getTopLeft();
	Coord bR = map.getBottomRight();
	std::cout << "Width: " << tL.second - bR.second << std::endl;
	std::cout << "Height: " << bR.first - tL.first << std::endl;
	return 0;
}
