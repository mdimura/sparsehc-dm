/*
 * matrix.h
 *
 *  Created on: Apr 9, 2013
 *      Author: kristin
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
using namespace std;

struct Element {
	uint row;
	uint col;
	float value;

	Element() {
		row = 0;
		col = 0;
		value = 0;
	}

	Element(uint aRow, uint aCol, float aValue) {
		row = aRow;
		col = aCol;
		value = aValue;
	}

	void update(uint aRow, uint aCol, float aValue) {
		row = aRow;
		col = aCol;
		value = aValue;
	}

	bool operator <(const Element &e) const {
		if(value==e.value) {
			if(row==e.row) {
				return col<e.col;
			} else return row<e.row;
		}
		else {
			return value < e.value;
		}
	}
	inline bool operator == (const Element& b) const
	{
	    return value == b.value;
	}
	static Element min_value()
	{
	    return Element(std::numeric_limits<uint>::min(),
			   std::numeric_limits<uint>::min(),
			   std::numeric_limits<float>::min());
	}
	static Element max_value()
	{
	    return Element(std::numeric_limits<uint>::max(),
			   std::numeric_limits<uint>::max(),
			   std::numeric_limits<float>::max());
	}

	static size_t size() {
		return sizeof(uint) * 2 + sizeof(float);
	}

	void print() {
		fprintf(stderr, "%u\t%u\t%f\n", row, col, value);
	}
};


//----------------------------------------------------------------------------------------------------

struct Matrix {

	size_t numElements;
	float threshold;
	uint numPoints;

	Matrix(uint numPoints, float threshold);

	Matrix();

	virtual ~Matrix() {
	}

	void clear() {
		elements.clear();
	}

	void push(uint row, uint col, float dist) {
		Element e(row, col, dist);
		elements.push_back(e);
	}

	double sparsity() {
		return (double) numElements / (numPoints * (numPoints - 1) / 2);
	}

	size_t sizeInBytes() {
		return elements.size() * Element::size();
	}

	void stats();
	size_t elementsCount() const {
		return elements.size();
	}
private:
	vector<Element> elements;
};

#endif /* MATRIX_H_ */
