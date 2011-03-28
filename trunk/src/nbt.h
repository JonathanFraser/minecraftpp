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
* @class tag
*	@brief super class for all tags
* 
*	the super class for all tags in an NBT file
* contains some printing functionality via virtual methods
* and because it is common to all tags the tag type member
*
* $Header $
*/
class Tag {
	protected:
		TAG_Type tagType;
		void printTabs(std::ostream&,unsigned int);
		Tag(TAG_Type);
		Tag(const Tag&);
		Tag& operator=(const Tag&);

	public:
	TAG_Type getTagType();
	virtual unsigned int write(uint8_t *&) = 0;
	virtual void print(std::ostream &, unsigned int = 0) = 0;
	virtual ~Tag() {}
};

/**
*	@class tagByte
*	@brief class for the TAG_Byte data type
*
* Stores only a single byte but here to keep a uniform interface
* with the tag system
*
* $Header $
*/
class TagByte : public Tag {
		TagByte(uint8_t *&data);
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagByte(const TagByte&);
		TagByte& operator=(const TagByte&);
	public:
	TagByte() : Tag(TAG_Byte) {};
	unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
		void print(std::ostream &output, unsigned int level = 0);
		float data;
};

class TagDouble : public Tag {
		friend Tag* getClass(uint8_t *&,TAG_Type);
		TagDouble(const TagDouble&);
		TagDouble& operator=(const TagDouble&);	
	public:
		TagDouble() : Tag(TAG_Double) {};
		unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
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
		unsigned int write(uint8_t *&);
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
		nbtFile();
		nbtFile(uint8_t *data);
		unsigned int write(uint8_t *data);
		TagCompound* getRootNode() { return root; }
		void print(std::ostream &output, unsigned int level = 0);
		~nbtFile();
};

#endif
