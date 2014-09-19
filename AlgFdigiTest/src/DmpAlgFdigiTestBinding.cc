#include <boost/python.hpp>
#include "DmpAlgFdigiTest.h"

BOOST_PYTHON_MODULE(libDmpAlgFdigiTest){
  using namespace boost::python;

  class_<DmpAlgFdigiTest,boost::noncopyable,bases<DmpVAlg> >("DmpAlgFdigiTest",init<>());
}
