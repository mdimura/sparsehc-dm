#ifndef DENDROGRAM_H_
#define DENDROGRAM_H_
/*
 * A 4 by (n−1) matrix Z is returned.
 * At the i-th iteration, clusters with indices Z[i, 0] and Z[i, 1] are combined
 * to form cluster n+i. A cluster with an index less than n corresponds to one
 * of the n original observations. The distance between clusters Z[i, 0] and Z[i, 1]
 * is given by Z[i, 2]. The fourth value Z[i, 3] represents the number of original
 * observations in the newly formed cluster.
 *
 */
#include <vector>
#include <unordered_map>
#include <fstream>
class Dendrogram {
private:
	struct Link {
		unsigned p1,p2;
		float distance;
		unsigned nump;
	};
	const unsigned numPoints;
	std::vector<Link> links;
	std::vector<unsigned> pointsCount; //number of points in the cluster
public:
	Dendrogram(unsigned numPoints):numPoints(numPoints)
	{
		links.reserve(numPoints-1);
		pointsCount.reserve(numPoints-1);
	}
	void add(unsigned p1, unsigned p2, float distance);
	void print(const std::string& fname) const
	{
		std::ofstream myfile;
		myfile.open (fname);
		for(const Link& l:links) {
			myfile<<l.p1<<' '<<l.p2<<' '<<l.distance<<' '<<l.nump<<'\n';
		}
	}
};

#endif //DENDROGRAM_H_
