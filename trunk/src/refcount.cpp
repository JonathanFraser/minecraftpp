#include "refcount.h"

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

Counter::Counter(const Counter &) {
	data->reference();
}

Counter::~Counter() {
	data->dereference();
}

unsigned int Counter::getCount() {
	return data->getCount();
}
