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


namespace dynamicgraph
{
  namespace sot
  {
    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Interpolator,"Interpolator");
    Interpolator::Interpolator( const std::string & name )
      : Entity(name),
        stampsSIN(NULL,"sotInterpolator("+name+")::input(vector)::stamp"),
        v0SIN(NULL,"sotInterpolator("+name+")::input(vector)::v0"),
        vnSIN(NULL,"sotInterpolator("+name+")::input(vector)::vn"),
        qsSIN(NULL,"sotInterpolator("+name+")::input(vector)::qs"),
        uSOUT( boost::bind(&Interpolator::compute_knots,this,_1,_2),
               stampsSIN<<v0SIN<<vnSIN<<qsSIN,
               "sotInterpolator("+name+")::output(vector)::u" ),
        psSOUT( boost::bind(&Interpolator::compute_control_points,this,_1,_2),
                stampsSIN<<v0SIN<<vnSIN<<qsSIN,
                "sotInterpolator("+name+")::output(vector)::ps" )
    {
      sotDEBUGIN(5);
      signalRegistration(stampsSIN<<v0SIN<<vnSIN<<qsSIN<<uSOUT<<psSOUT );
      sotDEBUGOUT(5);
    }

    ml::Vector& Interpolator::compute_knots(ml::Vector& res, int time)
    {
      return res;
    }

    ml::Vector& Interpolator::compute_control_points(ml::Vector& res, int time)
    {
      return res;
    }

  }
}


