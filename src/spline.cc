#include <cmath>
#include <time.h>
#ifndef WIN32
# include <sys/time.h>
#else
# include <jrl/mal/boost.hh>
# include <sot/core/utils-windows.hh>
# include <Winsock2.h>
#endif /*WIN32*/

#include <sot/curves/spline.h>
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




    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Spline,"Spline");
    Spline::Spline( const std::string & name )
      : Entity(name),
        tSIN(NULL,"sotSpline("+name+")::input(double)::input"),
        tSOUT( boost::bind(&Spline::compute,this,_1,_2),
                   tSIN,
                   "sotSpline("+name+")::output(double)::out" )
    {
      sotDEBUGIN(5);
      signalRegistration( tSIN<<tSOUT);

      sotDEBUGOUT(5);
    }


    double& Spline::compute(double& res, int time)
    {
      double t = tSIN(time);
      res = 3*t*t - 2*t*t*t;
      return res;

    }



  }
}


