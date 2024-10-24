#include <exception>
#include "BagIterator.h"
#include "Bag.h"

using namespace std;


BagIterator::BagIterator(const Bag& c): bag(c)
{
	this->currentPos = c.head;

	if (this->currentPos != -1) {
		this->currentFreq = 1;
	}
	else
		this->currentFreq = 0;
}
// BC = WC = TC = Theta(1)


void BagIterator::first() {
	this->currentPos = this->bag.head;

	if (this->currentPos != -1) {
		this->currentFreq = 1;
	}
	else
		this->currentFreq = 0;
}
// BC = WC = TC = Theta(1)


void BagIterator::next() {
	if (!this->valid()) {
		throw exception();
	}

	if (this->bag.elems[this->currentPos].second > this->currentFreq) {
		this->currentFreq++;
		return;
	}

	//means freq is equal, we can't increment it:
	this->currentPos = this->bag.next[this->currentPos];
	if (this->currentPos == -1)
		this->currentFreq = 0;
	else
		this->currentFreq = 1;
}
// BC = WC = TC = Theta(1)


bool BagIterator::valid() const {
	if (this->currentPos == -1)
		return false;

	if (this->bag.elems[this->currentPos] == NULL_TPAIR)
		return false;

	if (this->bag.elems[this->currentPos].second < this->currentFreq)
		return false;

	return true;
}
// BC = WC = TC = Theta(1)


TElem BagIterator::getCurrent() const {
	if (!this->valid()) {
		throw exception();
	}

	return this->bag.elems[this->currentPos].first;
}
// BC = WC = TC = Theta(1)