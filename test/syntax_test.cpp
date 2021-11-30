#include "shape.hpp"
#include <boost/ut.hpp>

using namespace boost::ut;
using namespace boost::ut::literals;
using namespace shp;

bool is_shape(shape2d auto shape) noexcept{
  return shape(0.0, 0.0);
}

int main(){
  "shape_test"_test = []{
    // some comments I don't know
    circle c1{1.0, 0.0, 0.0};
    circle c2{1.0, 0.0, 0.0};
    combine<2>(c1, c2);
  };
}
