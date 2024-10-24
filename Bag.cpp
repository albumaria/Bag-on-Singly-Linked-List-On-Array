#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>
#include <algorithm>

using namespace std;

Bag::Bag() {
	this->cap = 100;
	this->elems = new TPair[this->cap];
	this->next = new int[this->cap];
	this->head = -1;

	for (int i = 0; i < this->cap - 1; i++)
		this->next[i] = i + 1;

	fill(this->elems, this->elems + this->cap, NULL_TPAIR);
	this->next[this->cap - 1] = -1;
	this->firstEmpty = 0;
	this->bagSize = 0;
}
// BC = WC = TC = Theta(capacity)


void Bag::resize() {
	// if this function is called, we know the SDLLA is full.
	int oldcap = this->cap;
	this->cap *= 2;

	TPair* auxElems = new TPair[this->cap];
	int* auxNext = new int[this->cap];

	int i;

	for (i = 0; i < oldcap; i++) { //moving all the old elements into the new one
		auxElems[i] = this->elems[i];
		auxNext[i] = this->next[i];
	}
	this->firstEmpty = oldcap; //new firstempty is on the first position of the empties

	for (i = oldcap; i < this->cap; i++) { // setting it up
		auxElems[i] = NULL_TPAIR;
		auxNext[i] = i + 1;
	}

	auxNext[this->cap - 1] = -1;

	delete[] this->elems;
	delete[] this->next;
	this->elems = auxElems;
	this->next = auxNext;
}
// BC = WC = TC = Theta(new_capacity) or Theta(capacity) because new_capacity = capacity * 2


void Bag::add(TElem elem) {
	int i = this->head;

	while (i != -1) { // in case the element is already in the bag
		if (this->elems[i].first == elem) {
			this->elems[i].second++;
			this->bagSize++;
			return;
		}
		i = this->next[i];
	}

	//checking if we need to resize, if we do, resize:
	if (this->firstEmpty == -1) {
		this->resize();
	}

	//inserting on first empty pos
	int newPosition = this->firstEmpty;
	this->elems[newPosition] = TPair(elem, 1);
	this->firstEmpty = this->next[this->firstEmpty];
	this->next[newPosition] = this->head;
	this->head = newPosition;
	this->bagSize++;
}
// BC = Theta(1) if the element is already in the list, is first and we just need to increment the frequency
//		or when there is space for a new element and we just add it to the bag
// WC = Theta(bagSize) if we need to resize (as bagSize = capacity) or when the element is
//		already in the bag, but at the last positions of the bag
// TC = O(bagSize)


bool Bag::remove(TElem elem) {
	int current = this->head;
	int prev = -1;

	while (current != -1 and this->elems[current].first != elem) { // finding 2 positions where the first is on the elem
		prev = current;
		current = this->next[current];
	}

	if (current != -1) { // the element was found
		if (this->elems[current].second > 1) { // if we can decrement without having to remove
			this->elems[current].second--;
			this->bagSize--;
			return true;
		}

		else if (current == this->head) { // if we have to delete the head
			this->head = this->next[this->head];
		}
		else {
			this->next[prev] = this->next[current]; // if it's not the head, we don't need to update the head
		}
		this->elems[current] = NULL_TPAIR; // the removing
		this->next[current] = this->firstEmpty;
		this->firstEmpty = current;
		this->bagSize--;
		return true;
	}
	else
		return false; // else the element was not found
}
// BC = Theta(1) if the element to be deleted is on the first position
// WC = Theta(bagSize) if we have to search until the end to find the element to delete it
// TC = O(bagSize)


int Bag::removeElementsWithMultipleOccurences() {
	int nr_removed = 0; // to store the number of occurences
	int old_next; // to store the next after removing the element

	int current = this->head; // going through the SLLA with 2 positions
	int prev = -1;

	while (current != -1) { 
		if (this->elems[current].second > 1) // then we need to remove
		{
			nr_removed += this->elems[current].second;
			this->bagSize -= this->elems[current].second;

			if (current == this->head)
				this->head = this->next[this->head];
			else
				this->next[prev] = this->next[current];

			old_next = this->next[current];

			this->elems[current] = NULL_TPAIR; // the removing
			this->next[current] = this->firstEmpty;
			this->firstEmpty = current;
			this->bagSize;

			prev = current;
			current = old_next;
		}
		else { // go to the next if there's only one occurence
			prev = current;
			current = this->next[current];
		}
	}

	return nr_removed;
}
// BC = WC = TC = Theta(bagSize), because we have to go through the whole bag in all cases.


bool Bag::search(TElem elem) const {
	int i = this->head;

	while (i != -1) {
		if (this->elems[i].first == elem)
			return 1;

		i = this->next[i];
	}

	return 0;
}
// BC = Theta(1) if it is the first element in the bag
// WC = Theta(n) if we have to search up until the end to find the element or it is not in the bag
// TC = O(n)


int Bag::nrOccurrences(TElem elem) const {
	int i = this->head;

	while (i != -1) {
		if (this->elems[i].first == elem)
			return this->elems[i].second;

		i = this->next[i];
	}

	return 0;
}
// BC = Theta(1) if it is the first element in the bag
// WC = Theta(n) if we have to search up until the end to find the element or it is not in the bag
// TC = O(n)


int Bag::size() const {
	return this->bagSize;
}
// BC = WC = TC = Theta(1)


bool Bag::isEmpty() const {
	if (this->bagSize == 0)
		return 1;

	return 0;
}
// BC = WC = TC = Theta(1)


BagIterator Bag::iterator() const {
	return BagIterator(*this);
}
// BC = WC = TC = Theta(1)

Bag::~Bag() {
	delete[] this->elems;
	delete[] this->next;
}
// BC = WC = TC = Theta(capacity)

