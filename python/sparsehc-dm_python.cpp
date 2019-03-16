#include <memory>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/stl_iterator.hpp>
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

/*TODO: improve pushList() performance by introducing a buffer to first copy values
from Python::list and then push them to InMatrix in a separate thread.
*/
template<typename T>
void pushList(InMatrix& mat,const T& list, unsigned i/*,const unsigned rowLength*/)
{
	/*const unsigned n=boost::python::len(list);
	std::vector<float> vec;
	vec.resize(n);*/
	unsigned j=i+1;
	std::for_each(boost::python::stl_input_iterator<float>(list),
		       boost::python::stl_input_iterator<float>(),
		       [&mat,&i,&j](const float& v) {
				mat.push(Element(i,j++,v));
			 });
	/*std::copy(boost::python::stl_input_iterator<float>(list),
		   boost::python::stl_input_iterator<float>(),vec.begin());

	auto it=boost::python::stl_input_iterator<float>(list);
	while(it!=boost::python::stl_input_iterator<float>()) {
		for(unsigned j=i+1; j<rowLength; j++) {
			//std::cout<<"i="<<i<<" j="<<j<<" n="<<n<<std::endl;
			vec.emplace_back(i,j,*it++);
		}
		++i;
	}
	{ vec.emplace_back(i,i+1+j++,v);}
	mat.push(Element(i,i+1+j++,v));
	for(auto& e:vec) {
		mat.push(std::move(e));
	}*/
}
void (InMatrix::*push)(unsigned, unsigned, float) = &InMatrix::push;
void (InMatrix::*pushElement)(Element) = &InMatrix::push;

BOOST_PYTHON_MODULE(sparsehc_dm)
{
	boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
	using namespace boost::python;
	//class_<InMatrix,boost::noncopyable>("InMatrix",init<unsigned,const std::string,const std::string,unsigned long>());
	class_<InMatrix,boost::noncopyable>("InMatrix", init<unsigned long>())
			.def(init<optional<unsigned long>>())
			.def("push",push);
	def("linkage", linkage);
	//boost::python::list
	//boost::python::numeric::array
	def("push", pushList<boost::python::list>);

}
