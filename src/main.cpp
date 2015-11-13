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
inline float bad_rand()
{
	static std::random_device rd;
	static std::minstd_rand re(rd());
	static std::uniform_real_distribution<float> unif(0.0f,20.0f);
	static unsigned i=0;
	static float last=unif(re);
	if(++i%2==0) {
	  last*=0.95;
	} else {
	  last=unif(re);
	}
	return last;
}
////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
bool verbose = false;

int main(int argc, char **argv) {

	int linkage = 0;
	char treeFileName[BUF_SIZE];

	Timer timer;
	timer.tic();

	printf("\n============================== SparseHC ==============================\n");

	printUsage();

	fprintf(stderr, "\n==== Extract options ====\n");

	if (!extractOptions("--tree=%s", treeFileName, argc, argv))
		EXIT_MSG("The path to output tree file is required!");
	printf("Output tree: %s\n", treeFileName);

	char buf[BUF_SIZE];
	if (extractOptions("--linkage=%s", buf, argc, argv)) {
		if (strcmp(buf, "average") == 0) {
			linkage = 0;
			printf("Linkage: average\n");
		} else if (strcmp(buf, "complete") == 0) {
			linkage = 1;
			printf("Linkage: complete\n");
		} else if (strcmp(buf, "single") == 0) {
			linkage = 2;
			printf("Linkage: single\n");
		} else if (strcmp(buf, "weighted") == 0) {
			linkage = 3;
			printf("Linkage: weighted\n");
		} else
			EXIT_MSG( "Invalid linkage option! ");
	}
	initProfiler(1, "cluster");

	fprintf(stderr, "\n==== Read distance matrix from file ====\n");
	InMatrix * inMat;
	inMat = new InMatrix();

	std::cout<<"inserting values: "<<std::flush;
	const unsigned num_points=40000;
	for (unsigned p1 = 0; p1<num_points ; ++p1)
	{
	  for(unsigned p2 = p1+1; p2<num_points ; ++p2) {
		  inMat->push(p1,p2,bad_rand());
	  }
	  if(p1%(num_points/10)==0) {
	    std::cout<<p1*100/num_points<<"% "<<std::flush;
	  }
	}
	std::cout<<std::endl;
	inMat->sort();

	inMat->stats();

	fprintf(stderr, "\n==== Perform clustering ====\n");
	Cluster * cluster = NULL;

	if (linkage == 0)
		cluster = new AverageCluster(inMat->getNumPints(), treeFileName);
	else if (linkage == 1)
		cluster = new CompleteCluster(inMat->getNumPints(), treeFileName);
	else if (linkage == 2)
		cluster = new SingleCluster(inMat->getNumPints(), treeFileName);
	else if (linkage == 3)
		cluster = new AverageCluster(inMat->getNumPints(), treeFileName);

	cluster->createLeaves();
	PROFILE("cluster", cluster->clusterMatrix(inMat));

	delete inMat;
	delete cluster;

	printProfilerStats(); // print profile statistics before exit
	timer.toc();
	printf("Total time taken: = %.4lf seconds\n", timer.getLapse());
	return EXIT_SUCCESS;
}

