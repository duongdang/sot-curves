#include <cmath>
#include <time.h>
#ifndef WIN32
# include <sys/time.h>
#else
# include <jrl/mal/boost.hh>
# include <sot/core/utils-windows.hh>
# include <Winsock2.h>
#endif /*WIN32*/

#include <sot/curves/interpolator.h>
#include <sot/core/debug.hh>
#include <sot/core/macros-signal.hh>
#include <dynamic-graph/factory.h>
#include <fstream>

#include <dynamic-graph/entity.h>

namespace sot = dynamicgraph::sot;

namespace{
  bool check_close(const ml::Vector& v1, const ml::Vector& v2)
  {
    static const double EPSILON = 1e-9;
    if ( (v1.size() == 0) || (v1.size() != v2.size()) )
      {
        return false;
      }
    return (v1-v2).norm() < EPSILON;
  }
}

namespace dynamicgraph
{
  namespace sot
  {
    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Interpolator,"Interpolator");
    Interpolator::Interpolator( const std::string & name )
      : Entity(name),
        dimension_(3),stamps_(), v0_(3), vn_(3), qs_(),
        knots_(), normed_knots_(), cv_(),
        dimensionSIN(NULL,"sotInterpolator("+name+")::input(int)::dimension"),
        stampsSIN(NULL,"sotInterpolator("+name+")::input(vector)::stamp"),
        v0SIN(NULL,"sotInterpolator("+name+")::input(vector)::v0"),
        vnSIN(NULL,"sotInterpolator("+name+")::input(vector)::vn"),
        qsSIN(NULL,"sotInterpolator("+name+")::input(vector)::qs"),
        knotsSOUT( boost::bind(&Interpolator::compute_knots,this,_1,_2),
                   stampsSIN<<v0SIN<<vnSIN<<qsSIN,
                   "sotInterpolator("+name+")::output(vector)::knots" ),
        cvSOUT( boost::bind(&Interpolator::compute_control_points,this,_1,_2),
                stampsSIN<<v0SIN<<vnSIN<<qsSIN,
                "sotInterpolator("+name+")::output(vector)::cv" )
    {
      sotDEBUGIN(5);
      signalRegistration(dimensionSIN<<stampsSIN<<v0SIN<<vnSIN<<qsSIN<<knotsSOUT<<cvSOUT );
      sotDEBUGOUT(5);
    }

    ml::Vector& Interpolator::compute_knots(ml::Vector& res, int time)
    {
      res = knots_;
      return res;
    }

    ml::Vector& Interpolator::compute_control_points(ml::Vector& res, int time)
    {
      ml::Vector stamps = stampsSIN(time);
      ml::Vector v0 = v0SIN(time);
      ml::Vector vn = vnSIN(time);
      ml::Vector qs = qsSIN(time);
      int dimension = dimensionSIN(time);

      if ( !( check_close(stamps, stamps_) && check_close(v0, v0_) &&
              check_close(vn, vn_) && check_close(qs, qs_) &&
              dimension == dimension_
              ) )
        {
          dimension_ = dimension;
          stamps_ = stamps;
          v0_ = v0;
          vn_ = vn;
          qs_ = qs;
          normed_knots_ = knots_*(knots_(knots_.size()) - knots_(0));
          assert( 3*stamps.size() == qs.size() );
        }
      return res;
    }

  }
}


