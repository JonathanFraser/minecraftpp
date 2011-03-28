#include <stdint.h>
#include <string>
 
#ifndef __UTIL_H
#define __UTIL_H

/** takes a pointer into an array of bytes
  * converts the first 2 bytes of the array 
	* into a short, and increments the pointer 
	* by 2
  **/ 
int16_t arrayToShort(uint8_t *&data); 

/** takes a pointer into an array of bytes
  * converts the first 4 bytes of the array 
	* into an int, and increments the pointer 
	* by 4
  **/ 
int32_t arrayToInt(uint8_t *&data);

/** takes a pointer into an array of bytes
  * converts the first 8 bytes of the array 
	* into a Long, and increments the pointer 
	* by 8
  **/ 
int64_t arrayToLong(uint8_t *&data); 


/** takes a pointer into an array of bytes
  * converts the first 2 bytes of the array 
	* into a short, which is the length of the string
	* it then reads int he string, out of the reast of
	* the data, the string is not null terminated
	* it then increments the pointer 2 + stringlength
  **/ 
std::string arrayToString(uint8_t *&data);

float arrayToFloat(uint8_t *&data);
double arrayToDouble(uint8_t *&data);


unsigned int stringToArray(uint8_t *&,const std::string &);
void longToArray(uint8_t *&, int64_t);
void intToArray(uint8_t *&,int32_t);
void shortToArray(uint8_t *&,int16_t);
void floatToArray(uint8_t *&,float);
void doubleToArray(uint8_t *&,double);
#endif
