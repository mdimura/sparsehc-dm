#include "dendrogram.h"

void Dendrogram::add(unsigned p1, unsigned p2, float distance)
{
	Link l;
	l.p1=p1;
	l.p2=p2;
	l.distance=distance;
	l.nump=0;
	l.nump+=p1>=numPoints?pointsCount.at(p1-numPoints):1;
	l.nump+=p2>=numPoints?pointsCount.at(p2-numPoints):1;
	pointsCount.push_back(l.nump);
	links.push_back(l);
}

