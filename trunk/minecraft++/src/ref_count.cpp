#include "ref_count.h"
#include <algorithm>

Countee::Countee() : count(0) {
}

void Countee::reference() {
	if(!(count++)) {
		thaw();
	}
}

void Countee::dereference() {
	if(!(--count)) {
		freeze();
	}
}

unsigned int Countee::getCount() {
	return count;
}

Counter::Counter(Countee* data): data(data) {
	data->reference();
}

Counter::Counter(const Counter &B) : data(B.data) {
	data->reference();
}

void Counter::swap( Counter &B) {
	std::swap(data,B.data);
}

Counter::~Counter() {
	data->dereference();
}

unsigned int Counter::getCount() {
	return data->getCount();
}
