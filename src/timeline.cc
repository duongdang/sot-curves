#include <cmath>
#include <time.h>
#ifndef WIN32
# include <sys/time.h>
#else
# include <jrl/mal/boost.hh>
# include <sot/core/utils-windows.hh>
# include <Winsock2.h>
#endif /*WIN32*/

#include <sot/curves/timeline.h>
#include <sot/core/debug.hh>
#include <sot/core/macros-signal.hh>
#include <dynamic-graph/factory.h>
#include <fstream>

#include <dynamic-graph/entity.h>
#include <dynamic-graph/all-commands.h>

namespace sot = dynamicgraph::sot;


namespace dynamicgraph
{
  namespace sot
  {

    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Timeline,"Timeline");
    Timeline::Timeline( const std::string & name )
      : Entity(name),
        count_(0),
        state_(STOP),
        period_(0.005),
        timeMaxSIN("sotTimeline("+name+")::int(double)::timeMax" ),
        triggerSOUT("sotTimeline("+name+")::output(double)::dummy" ),
        scaledTimeSOUT("sotTimeline("+name+")::output(double)::scaledTime")
    {
      sotDEBUGIN(5);
      triggerSOUT.setFunction(boost::bind(&Timeline::triggerCall,this,_1,_2));
      //scaledTimeSOUT.setFunction(boost::bind(&Timeline::compute,this,_1,_2));

      signalRegistration(timeMaxSIN<<scaledTimeSOUT<<triggerSOUT);
      sotDEBUGOUT(5);

      using namespace command;

      addCommand("setState",
		 makeCommandVoid1(*this,&Timeline::setState,
				  docCommandVoid1("Set time line state (start|stop)",
						  "string (start|stop)")));


    }

    int& Timeline::triggerCall( int& dummy, int timeCurr )
    {
      if (state_ == START)
        {
          count_ += 1;
        }
      double timeMax = timeMaxSIN(timeCurr);
      double scaled_t = (double)(count_)*period_/timeMax;
      if (scaled_t > 1)
        {
          scaled_t = 1;
        }
      scaledTimeSOUT.setConstant(scaled_t);
      scaledTimeSOUT.setTime(timeCurr);
      return dummy;
    }

    double& Timeline::compute( double& res, int time)
    {
      double timeMax = timeMaxSIN(time);
      res = (double)(count_)*period_/timeMax;
    }


    void Timeline::setState(const std::string& statearg)
    {
      if( statearg == "start" ){ state_ = START; }
      else if( statearg == "stop" ){ state_ = STOP; }
    }

  }
}


