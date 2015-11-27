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
		push(Element(row,col,value));
	}
	void push(Element e) {
		threshold=threshold<e.value?e.value:threshold;
		sorter.push(std::move(e));
	}
	/*void push(float v) {
		threshold=threshold<v?v:threshold;
		sorter.push(std::move(e));
	}*/

	/*void pushvec(std::vector<float> v,unsigned i) {
		for(unsigned j=0; j<v.size(); ++j) {
			Element e{i,j+i+1,std::move(v[j])};
			threshold=threshold<e.value
				  ?nextafterf(e.value,e.value+100.0f):threshold;
			sorter.push(std::move(e));
		}
	}*/
	void sort() {
		threshold=nextafterf(threshold,threshold+100.0f);
		sorter.sort();
		numPoints=(1L+sqrt(1L+sorter.size()*8L))/2L;
	}
	unsigned getNumPints() const {

		return numPoints;
	}

	bool loadElements();

	bool isEmpty() {
		return (pos >= elementsCount() && sorter.empty());
	}

	//Element* getNext();

	bool getNext(uint &row, uint &col, float &value);
private:
	typedef stxxl::sorter<Element, Cmp> ElementSorter;
	ElementSorter sorter;
	//FILE * file;
}
;

#endif /* INMATRIX_H_ */
