/*
 * main.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: kristin
 */
#include "common.h"
#include "inmatrix.h"
#include "averagecluster.h"
#include "completecluster.h"
#include "singlecluster.h"
#include <memory>
#include <iostream>
#include <chrono>
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}
Dendrogram linkage(InMatrix& mat, const std::string& method) {
	std::unique_ptr<Cluster> cluster;
	if (method == "complete")
		cluster = make_unique<CompleteCluster>(mat.getNumPints());
	else if (method == "single")
		cluster = make_unique<SingleCluster>(mat.getNumPints());
	else if (method == "average")
		cluster = make_unique<AverageCluster>(mat.getNumPints());
	else
		cluster = make_unique<AverageCluster>(mat.getNumPints());
	cluster->createLeaves();
	return cluster->clusterMatrix(&mat);
}


////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
bool verbose = false;

int main(int argc, char **argv) {

	char treeFileName[BUF_SIZE];
	if (!extractOptions("--tree=%s", treeFileName, argc, argv))
		EXIT_MSG("The path to output tree file is required!");

	char method[32];
	if (!extractOptions("--linkage=%s", method, argc, argv))
		strcat(method,"complete");

	InMatrix inMat(0,"sorter.tmp","syscall nodirect unlink",64*1024*1024);

	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	using time_point=std::chrono::steady_clock::time_point;
	using std::chrono::steady_clock;
	unsigned i,j;
	float v;
	time_point startRead = steady_clock::now();
	while(std::cin>>i) {
		std::cin>>j;
		std::cin>>v;
		inMat.push(i,j,v);
	}
	time_point finishedRead = steady_clock::now();
	double reading=duration_cast<milliseconds>(finishedRead - startRead).count()/1000.0;
	std::cout<<"Reading RMSDs took: "<<reading<<" s"<<std::endl;
	inMat.sort();
	time_point finishedSort = steady_clock::now();
	double sorting=duration_cast<milliseconds>(finishedSort-finishedRead).count()/1000.0;
	std::cout<<"Sorting took: "<<sorting<<" s"<<std::endl;
	linkage(inMat,method).print(treeFileName);
	time_point finishedClust = steady_clock::now();
	double clustering=duration_cast<milliseconds>(finishedClust-finishedSort).count()/1000.0;
	std::cout<<"Clustering took: "<<clustering<<" s"<<std::endl;

	return 0;
}

