#include <stdint.h>
#include <string>
 
#ifndef __UTIL_H
#define __UTIL_H

/** takes a pointer into an array of bytes
  * converts the first 2 bytes of the array 
	* into a short, and increments the pointer 
	* by 2
	*
	* @param data pointer to a bigendian encoded int16_t
	*
  **/ 
int16_t arrayToShort(uint8_t *&data); 

/** takes a pointer into an array of bytes
  * converts the first 4 bytes of the array 
	* into an int, and increments the pointer 
	* by 4
	*
	*	@param data pointer to a bigendian encoded int32_t
	*
  **/ 
int32_t arrayToInt(uint8_t *&data);

/** takes a pointer into an array of bytes
  * converts the first 8 bytes of the array 
	* into a Long, and increments the pointer 
	* by 8
	*
	*	@param data pointer to a bigendian encoded int64_t
	*
  **/ 
int64_t arrayToLong(uint8_t *&data); 


/** takes a pointer into an array of bytes
  * converts the first 2 bytes of the array 
	* into a short, which is the length of the string
	* it then reads int he string, out of the reast of
	* the data, the string is not null terminated
	* it then increments the pointer 2 + stringlength
	*
	*	@param data pointer to first a bigendian int16_t 
	*             and then a non NULL terminated string
	*
  **/ 
std::string arrayToString(uint8_t *&data);

/**
  * takes a pointer into an array of bytes
  * converts the first 4 bytes of the array
  * into a float and increments the pointer
	* by 4
	*
	* @param data pointer to an IEEE bigendian float 
  *
	**/
float arrayToFloat(uint8_t *&data);

/**
  * takes a pointer into an array of bytes
  * converts the first 8 bytes of teh array
  * into a double and increments the pointer
  * by 8
	*
  * @param data pointer to an IEEE bigendian double
  *
  **/
double arrayToDouble(uint8_t *&data);

/** takes a pointer to an array and a string reference
  * inserts the length of the string via shortToArray
  * then writes the string to the array non null terminated
  *
  * @param data pointer to a large enough data array to store string and size
	*
	* @param text string to insert into the array
  *
  **/
unsigned int stringToArray(uint8_t *&data,const std::string &text);

/** takes a pointer to an array and inserts an int64_t into it
  * the int is stored in big endian, also increments pointer by 8
  *
  * @param data pointer to the array
	*
	* @param number value to place in array
  *
  **/
void longToArray(uint8_t *&data, int64_t number);


/** takes a pointer to an array and inserts an int32_t into it
  * the int is stored in big endian, also increments pointer by 4
  *
  * @param data pointer to an array
	*
	* @param number number to insert
  *
  **/
void intToArray(uint8_t *&data,int32_t number);

/** takes a pointer to an array and inserts an int16_t into it
  * the int is stored in big endian, also increments the pointer by 2
  *
  * @param data pointer to array
	*
	* @param number number to insert
  *
  **/
void shortToArray(uint8_t *&data,int16_t number);

/** takes a pointer to an array and inserts a big endian
  * IEEE float into, also increments the pointer by 4
  *
  * @param data pointer to array
  *
  * @param number the float to insert
  *
  **/
void floatToArray(uint8_t *&data,float number);

/** takes a pointer to an array and inserts a big endian
  * IEEE double into it, also increments the pointer by 8
  *
  * @param data pointer to array
  *
	* @param number number to insert
  *
  **/
void doubleToArray(uint8_t *&,double);
#endif
