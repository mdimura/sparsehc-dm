/*
 * cluster.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: kristin
 */

#include "cluster.h"
#include "common.h"
#include <boost/graph/graphviz.hpp>

void Vertex::updateAncestor(Vertex* ancestor) {
	this->ancestor = ancestor;
	if (this->left)
		this->left->updateAncestor(ancestor);
	if (this->right)
		this->right->updateAncestor(ancestor);
}

//----------------------------------------------------------------------------------------------------

Cluster::Cluster(uint n) :
		numLeaves(n),dendrogram(n){
	// add the leaves into the final graph
}

void Cluster::createLeaves() {

	for (uint i = 0; i < numLeaves; ++i)
		vertices.push_back(this->createVertex(i));
}

Cluster::~Cluster() {

	for (uint i = 0; i < vertices.size(); ++i)
		delete vertices[i];

}

void Cluster::updateGraph(uint id1, uint id2, float dist) {
		dendrogram.add(id1 ,id2 ,dist);
}

