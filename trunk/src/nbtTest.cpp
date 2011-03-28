#include <stdint.h>
#include <fstream>
#include <iostream>

#include "nbt.h"


int main(void) {
	std::ifstream input("data/bigtest.nbt",std::ios_base::in);
	input.seekg(0,std::ios::end);
	uint64_t length = input.tellg();
	input.seekg(0,std::ios::beg);
	uint8_t data[length];
	uint8_t data2[length];
	input.read(reinterpret_cast<char *>(data),length);
	int64_t count = nbtFile(data).write(data2);
	nbtFile(data2).print(std::cout);
	
	std::cout << length - count << std::endl;
	
	return 0;
}
