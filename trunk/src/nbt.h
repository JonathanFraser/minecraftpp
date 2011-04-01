#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>

#ifndef __NBT_H
#define __NBT_H

// NBT tag type enum
enum TAG_Type {
	TAG_End,
	TAG_Byte,
	TAG_Short,
	TAG_Int,
	TAG_Long,
	TAG_Float,
	TAG_Double,
	TAG_ByteArray,
	TAG_String,
	TAG_List,
	TAG_Compound
};

/**
* @class Tag
*	@brief super class for all tags
* 
*	the super class for all tags in an NBT file
* contains some printing functionality via virtual methods
* and because it is common to all tags the tag type member
*
*/
class Tag {
	protected:
		TAG_Type tagType;
		void printTabs(std::ostream&,unsigned int);
		Tag(TAG_Type);
		Tag(const Tag&);
		Tag& operator=(const Tag&);

	public:
	/**
	*
	*	Get tag type
	*
	*	@return the type of tag
	*	
	*/
	TAG_Type getTagType();

	/**
	*
	* a virtual method to allow subclasses to write their data out in 
	* nbt format
	*
	* @param datapointer an index into an array to dump the tags data, will
	*	increment the datapointer to the element after this tags data
	*
	* @param size the freespace available
	*/
	virtual size_t write(uint8_t *&datapointer, size_t size) = 0;

	/**
	* dumps the tags data in a semi readable format
	*
	* @param output a stream reference where to dump the data
	*
	* @param depth used internally to tab in recursive levels
	**/
	virtual void print(std::ostream &output, unsigned int depth = 0) = 0;
	virtual ~Tag() {}
};

/**
*	@class TagByte
*	@brief class for the TAG_Byte data type
*
* Stores only a single byte but here to keep a uniform interface
* with the tag system
*
*/
class TagByte : public Tag {
		TagByte(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagByte(const TagByte&);
		TagByte& operator=(const TagByte&);
	public:
	TagByte() : Tag(TAG_Byte) {};
	size_t write(uint8_t *&,size_t);
	void print(std::ostream &output, unsigned int level = 0);
	int8_t data;
};

class TagShort : public Tag {
		TagShort(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagShort(const TagShort&);
		TagShort& operator=(const TagShort&);
	public:
		TagShort() : Tag(TAG_Short) {};
		size_t write(uint8_t *&,size_t);
		void print(std::ostream &output, unsigned int level = 0);
		int16_t data;	
};

class TagInt : public Tag {
		TagInt(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagInt(const TagInt&);
		TagInt& operator=(const TagInt&);
	public:
		TagInt() : Tag(TAG_Int) {};
		size_t write(uint8_t *&,size_t);
		void print(std::ostream &output, unsigned int level = 0);
		int32_t data;
};

class TagLong : public Tag {
		TagLong(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagLong(const TagLong&);
		TagLong& operator=(const TagLong&);
	public:
		TagLong() : Tag(TAG_Long) {};
		size_t write(uint8_t *&,size_t);
		void print(std::ostream &output, unsigned int level = 0);
		int64_t data;
};

class TagFloat : public Tag {
		TagFloat(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagFloat(const TagFloat&);
		TagFloat& operator=(const TagFloat&);
	public:
		TagFloat() : Tag(TAG_Float) {};
		size_t write(uint8_t *&,size_t);
		void print(std::ostream &output, unsigned int level = 0);
		float data;
};

class TagDouble : public Tag {
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagDouble(const TagDouble&);
		TagDouble& operator=(const TagDouble&);	
	public:
		TagDouble() : Tag(TAG_Double) {};
		size_t write(uint8_t *&,size_t);
		void print(std::ostream &output, unsigned int level = 0);
		double data;
		TagDouble(uint8_t *&data);
};

class TagByteArray : public Tag {
		TagByteArray(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagByteArray(const TagByteArray&);
		TagByteArray& operator=(const TagByteArray&);
	public:
		uint32_t size;
		int8_t *data;	
		TagByteArray(uint32_t size);
		size_t write(uint8_t *&,size_t);
		~TagByteArray();
		void print(std::ostream &output, unsigned int level = 0);
};

class TagString : public Tag {
		TagString(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagString(const TagString&);
		TagString& operator=(const TagString&);
	public:
		TagString() : Tag(TAG_String) {};
		size_t write(uint8_t *&,size_t);
		void print(std::ostream &output, unsigned int level = 0);
		std::string data;	
};

class TagList : public Tag {
		TagList(uint8_t *&data);
		TAG_Type listType;
		std::vector<Tag*> tags;
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagList(const TagList&);
		TagList& operator=(const TagList&);
	public:
		TagList() : Tag(TAG_List) {};
		size_t write(uint8_t *&,size_t);
		Tag* operator[](unsigned int index);
		void print(std::ostream &output, unsigned int level = 0);
		~TagList();
};
typedef std::unordered_map<std::string,Tag*> tag_map;
class TagCompound : public Tag {
		TagCompound(uint8_t *&data);
		tag_map tags;
		friend Tag* getClass(uint8_t *&,TAG_Type);
		friend class nbtFile;
		TagCompound(const TagCompound&);
		TagCompound& operator=(const TagCompound&);
	public:
		TagCompound();
		size_t write(uint8_t *&,size_t);
		void insertSubTag(const std::string&, Tag*);
		Tag* getSubTag(const std::string&);
		void print(std::ostream &output, unsigned int level = 0);
		~TagCompound();
};



class nbtFile {
		TagCompound* root;
		std::string rootName;
		nbtFile(const nbtFile&);
		nbtFile& operator=(const nbtFile&);
	public:
		/*
		* Constructor for the nbtFile which initializes with and empty TagCompound
		*/
		nbtFile();

		/**
		* Constructor for an nbt file class which takes in previously read data
		*
		* @param data pointer to a data array containing a decompressed nbtFile
		*
		*/
		nbtFile(uint8_t *data);

		/**
		* writes the NBT data tree into the provided array
		* 
		*	@param data pointer to empty array for the tree
		*
		* @param size size available in the array (used to throw an assert)
		* 
		* @return The number of bytes written to the array
		*/
		size_t write(uint8_t *data,size_t size);
		
		/**
		*	returns a pointer to the root TagCompound
		*
		* @return the TagCompound at the root fo the tree
		*/
		TagCompound* getRootNode() { return root; }

		/**
		* dumps the contents in a semi sane format mostly useful for debugging
		*
		* @param output an output stream to dump the info
		*
		* @param data the indent depth to print to
		*/
		void print(std::ostream &output, unsigned int level = 0);

		/**
		* Destructor nuff said
		*/
		~nbtFile();
};

#endif
