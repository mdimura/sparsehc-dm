#include <memory>
#include <boost/python.hpp>
#include <boost/python/numeric.hpp>
#include "common.h"
#include "inmatrix.h"
#include "averagecluster.h"
#include "completecluster.h"
#include "singlecluster.h"

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
	return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}
boost::python::list linkage(InMatrix& mat, const std::string& method) {
	mat.sort();
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
	Dendrogram d=cluster->clusterMatrix(&mat);
	boost::python::list v;
	for(int r=0; r<mat.getNumPints()-1;++r) {
		boost::python::list row;
		Dendrogram::Link l=d.getLink(r);
		row.append<unsigned>(l.p1);
		row.append<unsigned>(l.p2);
		row.append<float>(l.distance);
		row.append<unsigned>(l.nump);
		v.append<boost::python::list>(row);
	}
	return v;

}

void pushList(InMatrix& mat,const boost::python::list& list, int i)
{
	Element e;
	for(unsigned j=0; j<boost::python::len(list); ++j) {
		float v=boost::python::extract<float>(list[j]);
		e.update(i,j+i+1,v);
		mat.push(e);
	}
}
void pushNp(InMatrix& mat,const boost::python::numeric::array& list, int i)
{
	Element e;
	for(unsigned j=0; j<boost::python::len(list); ++j) {
		float v=boost::python::extract<float>(list[j]);
		e.update(i,j+i+1,v);
		mat.push(e);
	}
}
void (InMatrix::*push)(unsigned, unsigned, float) = &InMatrix::push;
void (InMatrix::*pushElement)(Element) = &InMatrix::push;

BOOST_PYTHON_MODULE(sparsehc_dm)
{
	boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
	using namespace boost::python;
	//class_<InMatrix,boost::noncopyable>("InMatrix",init<unsigned,const std::string,const std::string,unsigned long>());
	class_<InMatrix,boost::noncopyable>("InMatrix")
			.def("push",push);
	def("linkage", linkage);
	def("push", pushNp);

}
