#include "util.h"

int16_t arrayToShort(uint8_t *&data) {
	uint16_t retval=0;
	for(uint8_t i=0;i<2;i++) {
		uint16_t datavel = *data++;
		retval |= static_cast<uint16_t>(datavel << (1-i)*8);
	}
	return retval;
}

int32_t arrayToInt(uint8_t *&data) {
	uint32_t retval = 0;
	for(uint64_t i=0;i<4;i++) {
		uint32_t temp = *data++;
		retval |= (temp << (3-i)*8);
	}

	return retval;
}

int64_t arrayToLong(uint8_t *&data) {
	uint64_t retval = 0;
	
	for(uint64_t i=0;i<8;i++) {
		uint64_t temp = *data++;
		retval |= (temp << (7-i)*8);
	}
	return retval;
}

std::string arrayToString(uint8_t *&data) {
	uint16_t size = arrayToShort(data);
	char buffer[size+1];
	for(uint64_t i=0;i<size;i++)
		buffer[i]=*data++;
	buffer[size]='\0';
	return std::string(buffer);
}

float arrayToFloat(uint8_t *&data) {
	uint32_t dummy = 0;
	for(uint8_t i = 0;i<4;i++) {
		uint32_t temp = *data++;
		dummy |= (temp << (3-i)*8);
	}	
	return *reinterpret_cast<float *>(&dummy);
}

double arrayToDouble(uint8_t *&data) {
	uint64_t dummy = 0;

	for(uint8_t i=0;i<8;i++) {
		uint64_t temp = *data++;
		dummy |= (temp << (7-i)*8);
	}	
	
	return *reinterpret_cast<double *>(&dummy);
}

unsigned int stringToArray(uint8_t *&data,const std::string &text) {
	uint16_t size = static_cast<uint16_t>(text.size());
	shortToArray(data,size);
	for(uint16_t i=0;i<size;i++) {
	 *data++ = text[i];
	}
	return size+2;
}

void shortToArray(uint8_t *&data,int16_t value) {
	for(uint8_t i=0;i<2;i++) {
		*data++ = static_cast<uint8_t>((value >> (1-i)*8) & 0x00FF); 
	}
}

void intToArray(uint8_t *&data, int32_t value) {
	for(uint8_t i=0;i<4;i++) {
		*data++ = static_cast<uint8_t>((value >> (3-i)*8) & 0x000000FF); 		
	}
}

void longToArray(uint8_t *&data, int64_t value) {
	for(uint8_t i=0;i<8;i++) {
		*data++ = static_cast<uint8_t>((value >> (7-i)*8) & 0x00000000000000FF); 
	}	
}

void floatToArray(uint8_t *&data,float value) {
	uint32_t dummy = *reinterpret_cast<uint32_t*>(&value);
	for(uint8_t i=0;i<4;i++) {
		*data++ = (dummy >> (3-i)*8) & 0x000000FF;
	}	
}

void doubleToArray(uint8_t *&data,double value) {
	uint64_t dummy = *reinterpret_cast<uint64_t*>(&value);
	for(uint8_t i=0;i<8;i++) {
		*data++ = (dummy >> (7-i)*8) & 0x00000000000000FF;
	}		
}
