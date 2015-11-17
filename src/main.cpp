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
#include <Eigen/Dense>
struct Point {
	static std::minstd_rand re;
	static std::uniform_real_distribution<float> unif;
	Point() {
		for(int i=0; i<3; i++) {
			r[i]=unif(re);
		}
		for(int i=3; i<n; i++) {
			r[i]=r[i-3]+0.1;
		}
	}
	static const unsigned n=3*150;
	const float invn=3.0f/n;
	Eigen::Matrix<float,n,1> r;
	float rmsd(const Point& o) const {
		return sqrt((r-o.r).squaredNorm()*invn);
	}
};
std::minstd_rand Point::re(0);
std::uniform_real_distribution<float> Point::unif(0.0f,100.0f);

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

	unsigned num_points;
	if (!extractOptions("-N=%u", &num_points, argc, argv)) {
		num_points=10000;
	}
	printf("num_points: %u\n", num_points);

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
	InMatrix inMat(num_points,"sorter.tmp","syscall nodirect unlink",64*1024*1024);

	std::vector<Point> points(num_points);
	std::cout<<"inserting values: "<<std::flush;
	for (unsigned p1 = 0; p1<num_points ; ++p1)
	{
		for(unsigned p2 = p1+1; p2<num_points ; ++p2) {
			inMat.push(p1,p2,points[p1].rmsd(points[p2]));
		}
		if(p1%(num_points/10)==0) {
			std::cout<<p1*100/num_points<<"% "<<std::flush;
		}
	}
	std::cout<<std::endl;
	inMat.sort();

	inMat.stats();

	fprintf(stderr, "\n==== Perform clustering ====\n");
	Cluster * cluster = NULL;

	if (linkage == 0)
		cluster = new AverageCluster(inMat.getNumPints(), treeFileName);
	else if (linkage == 1)
		cluster = new CompleteCluster(inMat.getNumPints(), treeFileName);
	else if (linkage == 2)
		cluster = new SingleCluster(inMat.getNumPints(), treeFileName);
	else if (linkage == 3)
		cluster = new AverageCluster(inMat.getNumPints(), treeFileName);

	cluster->createLeaves();
	PROFILE("cluster", cluster->clusterMatrix(&inMat));

	delete cluster;

	printProfilerStats(); // print profile statistics before exit
	timer.toc();
	printf("Total time taken: = %.4lf seconds\n", timer.getLapse());
	return EXIT_SUCCESS;
}

