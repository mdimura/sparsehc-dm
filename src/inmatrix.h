/*
 * inmatrix.h
 *
 *  Created on: Apr 25, 2013
 *      Author: kristin
 */

#ifndef INMATRIX_H_
#define INMATRIX_H_

#include "matrix.h"
#include "common.h"

#include <stxxl/sorter>

#include <string>
using namespace std;
struct Cmp
{
    bool operator () (const Element& a, const Element& b) const
    {
	return a < b;
    }
    static Element min_value()
    {
	return Element::min_value();
    }
    static Element max_value()
    {
	return Element::max_value();
    }
};
struct InMatrix: public Matrix {

	uint pos;


	InMatrix(unsigned numpoints=0, const std::string cachefile="sorter.tmp",
		 const std::string cacheOptions="syscall nodirect unlink",
		 unsigned long ramUse=1L*1024*1024*1024);

	~InMatrix() {
		pos = 0;
	}

	void push(uint row, uint col, float value) {
		Element e(row,col,value);
		sorter.push(e);
		threshold=threshold<value?value:threshold;
	}
	void sort() {
		sorter.sort();
		numPoints=sqrt(1L+sorter.size()*8L)/2L+1;
	}
	unsigned getNumPints() const {
		return numPoints;
	}

	bool loadElements();

	bool isEmpty() {
		return (pos >= elements.size() && sorter.empty());
	}

	Element* getNext();

	bool getNext(uint &row, uint &col, float &value);
private:
	typedef stxxl::sorter<Element, Cmp> ElementSorter;
	ElementSorter sorter;
	//FILE * file;
}
;

#endif /* INMATRIX_H_ */
