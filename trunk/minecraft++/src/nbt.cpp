#include <cassert>

#include "nbt.h"
#include "util.h"

Tag* getClass(uint8_t *&data,TAG_Type type) {
	switch(type) {
		case TAG_End:
			return NULL;
		case TAG_Byte:
			return new TagByte(data);
		case TAG_Short:
			return new TagShort(data);
		case TAG_Int:
			return new TagInt(data);
		case TAG_Long:
			return new TagLong(data);
		case TAG_Float:
			return new TagFloat(data);
		case TAG_Double:
			return new TagDouble(data);
		case TAG_ByteArray:
			return new TagByteArray(data);
		case TAG_String:
			return new TagString(data);
		case TAG_List:
			return new TagList(data);
		case TAG_Compound:
			return new TagCompound(data);
	}
	assert(false);
	return NULL;
}

Tag::Tag(TAG_Type B) : tagType(B) {
}

TAG_Type Tag::getTagType() {
	return tagType;
}

void Tag::printTabs(std::ostream &output,unsigned int level) {
	while(level-- >0) {
		output << "  ";
	}
}

TagByte::TagByte(uint8_t *&data) : Tag(TAG_Byte) {
	this->data = *data++;
}

size_t TagByte::write(uint8_t *&data,size_t size) {
	assert(size >= 1);
	*data++ = this->data;
	return 1;
}

void TagByte::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "Byte: " << static_cast<int>(data) << std::endl;
}

TagShort::TagShort(uint8_t *&data) :Tag(TAG_Short) {
	this->data = arrayToShort(data);
}

size_t TagShort::write(uint8_t *&data,size_t size) {
	assert(size >= 2);
	shortToArray(data,this->data);
	return 2;	
}

void TagShort::print(std::ostream& output, unsigned int level) {
	printTabs(output,level);
	output << "Short: " << static_cast<int>(data) << std::endl;
}

TagInt::TagInt(uint8_t *&data) : Tag(TAG_Int) {
	this->data = arrayToInt(data);
}

size_t TagInt::write(uint8_t *&data,size_t size) {
	assert(size >= 4);
	intToArray(data,this->data);
	return 4;
}

void TagInt::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "Int: " << static_cast<int>(data) << std::endl;
}

TagLong::TagLong(uint8_t *&data) : Tag(TAG_Long) {
	this->data = arrayToLong(data);
}

size_t TagLong::write(uint8_t *&data,size_t size) {
	assert(size >= 8);
	longToArray(data,this->data);
	return 8;
}

void TagLong::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "Long: " << static_cast<long int>(data) << std::endl;
}

TagFloat::TagFloat(uint8_t *&data) : Tag(TAG_Float) {
	this->data = arrayToFloat(data);
}

size_t TagFloat::write(uint8_t *&data, size_t size) {
	assert(size >= 4);
	floatToArray(data,this->data);
	return 4;
}

void TagFloat::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "Float: " << data << std::endl;
}

TagDouble::TagDouble(uint8_t *&data) : Tag(TAG_Double) {
	this->data = arrayToDouble(data); 
}

size_t TagDouble::write(uint8_t *&data,size_t size) {
	assert(size >= 8);
	doubleToArray(data,this->data);
	return 8;	
}

void TagDouble::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output <<  "Double: " << data << std::endl;
}

TagByteArray::TagByteArray(uint32_t size) : Tag(TAG_ByteArray), size(size), data(new int8_t[size]) {
}

TagByteArray::TagByteArray(uint8_t *&data) :Tag(TAG_ByteArray) {
	size = arrayToInt(data);
	this->data = new int8_t[size];
	for(uint32_t i = 0;i<size;i++)
		this->data[i] = *data++; 
}

size_t TagByteArray::write(uint8_t *&data,size_t size) {
	assert(size >= this->size + 4);
	intToArray(data,size);
	for(uint32_t i=0;i<size;i++) {
		*data++ = this->data[i];
	}
	return this->size+4;
}


void TagByteArray::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "Byte Array: [" << (int) size << " bytes]" << std::endl;
}

TagByteArray::~TagByteArray() {
	delete[] data;
}

TagString::TagString(uint8_t *&data) : Tag(TAG_String) {
	this->data = arrayToString(data);
}

size_t TagString::write(uint8_t *&data,size_t size) {
	assert(size >= 2+this->data.size());
	return stringToArray(data,this->data);
}

void TagString::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "String: " << data << std::endl;
}

TagList::TagList(uint8_t *&data) : Tag(TAG_List) {

	listType = static_cast<TAG_Type>(*data++);

	int32_t list_size = arrayToInt(data);

	for(int32_t i = 0;i<list_size;i++) {
		tags.push_back(getClass(data,listType));
	}

}

size_t TagList::write(uint8_t *&data,size_t size) {
	size_t count = 1;
	assert(size >= 5);
	*data++ = listType;
	intToArray(data,tags.size());
	count += 4;
	for(uint32_t i = 0;i<tags.size();i++) {
		count += tags[i]->write(data,size-count);
	}
	return count;
}

Tag* TagList::operator[](unsigned int index) {
	assert(index < tags.size());
	return tags[index];
}

void TagList::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "Tag List: " << std::endl;
	for(uint32_t i=0;i<tags.size();i++)
	{
		tags[i]->print(output,level+1);
	}
}

TagList::~TagList() {
	for(uint32_t i = 0 ;i<tags.size();i++) {
		delete tags[i];
	}
}

TagCompound::TagCompound() :Tag(TAG_Compound){
}

TagCompound::TagCompound(uint8_t *&data) : Tag(TAG_Compound) {
	Tag* data_pointer=NULL;
	do {
		TAG_Type type = static_cast<TAG_Type>(*data++);
		if(type == TAG_End)
			break;	
		std::string name = arrayToString(data);
		data_pointer = getClass(data,type);
		tags.insert(std::pair<std::string,Tag*>(name,data_pointer));
	} while(true);
}

size_t TagCompound::write(uint8_t *&data,size_t size) {
	size_t count = 0;
	for(tag_map::iterator i = tags.begin();i != tags.end(); i++ ) {
		assert(size-count >= 1);
		*data++ = i->second->getTagType();
		count++;
		assert(size-count >= (i->first.size()+2));
		count += stringToArray(data,i->first);
		count += i->second->write(data,size-count);	
	}
	assert(size-count >=1);
	*data++ = TAG_End;
	count++;
	return count;
}

void TagCompound::print(std::ostream &output, unsigned int level) {
	printTabs(output,level);
	output << "Compound: " << std::endl;
	for(tag_map::iterator i = tags.begin();i != tags.end(); i++ ) {
		 printTabs(output,level+1);
			output << i->first.c_str() << std::endl;
			i->second->print(output,level+1); 
			output << std::endl;
	}
}

void TagCompound::insertSubTag(const std::string &name,Tag* data_pointer) {
		tags.insert(std::pair<std::string,Tag*>(name,data_pointer));
}

Tag* TagCompound::getSubTag(const std::string &name){
	tag_map::iterator i = tags.find(name);
	if(i == tags.end())
		return NULL;
	
	return i->second; 
}

TagCompound::~TagCompound() {
	for(tag_map::iterator i = tags.begin();i != tags.end();i++) {
		delete (*i).second;
	}
}

nbtFile::nbtFile() : root(new TagCompound()), rootName("") {
}

nbtFile::nbtFile(uint8_t *data) {
	assert(*data == TAG_Compound);
	++data;
	rootName = arrayToString(data);
	root = new TagCompound(data);
}

size_t nbtFile::write(uint8_t *data,size_t size) {
	size_t count =0;
	assert(size >= 1);
	*data++ = TAG_Compound;
	count++;
	assert(size-count >= rootName.size()+2);
	count += stringToArray(data,rootName);
	count += root->write(data,size-count);
	return count;
}

void nbtFile::print(std::ostream &output, unsigned int level) {
	output << "NBT FILE NAMED: " << rootName << std::endl;
	root->print(output,level);
	output << std::endl;
}

std::string nbtFile::getRootName() {
	return rootName;
}

nbtFile::~nbtFile() {
	delete root;
}
 
