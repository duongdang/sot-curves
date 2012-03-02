#include <cmath>
#include <time.h>
#ifndef WIN32
# include <sys/time.h>
#else
# include <jrl/mal/boost.hh>
# include <sot/core/utils-windows.hh>
# include <Winsock2.h>
#endif /*WIN32*/

#include <sot/curves/bezier.h>
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



    namespace{
      void getLine(const ml::Matrix& m, int line_no,
                   ml::Vector& res)
      {
        if (line_no < 0 || line_no > (int)m.nbRows())
          {
            return;
          }
        for (unsigned j = 0; j < m.nbCols(); j++)
          {
            res(j) = m.elementAt(line_no, j);
          }

      }
    }


    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Bezier,"Bezier");
    Bezier::Bezier( const std::string & name )
      : Entity(name),
        tSIN(NULL,"sotBezier("+name+")::input(double)::t"),
        controlPointsSIN(NULL,"sotBezier("+name+")::input(matrix)::controlPoints"),
        stateSOUT( boost::bind(&Bezier::computeState,this,_1,_2),
                   controlPointsSIN<<tSIN,
                   "sotBezier("+name+")::output(vector)::state" )
    {
      sotDEBUGIN(5);
      signalRegistration( controlPointsSIN<<tSIN<<stateSOUT);

      sotDEBUGOUT(5);
    }


    void Bezier::compute_bezier(ml::Matrix points,
                                const double& t, ml::Vector& result)
    {
      result.resize(points.nbCols());

      // Recursive formula
      if (points.nbRows() == 1)
        {
        getLine(points, 0, result);
        return;
        }
      ml::Vector B_0_n1, B_1_n;
      ml::Matrix points_0_n1 = points.extract(0,0, (int)points.nbRows() - 1,
                                              (int)points.nbCols());
      ml::Matrix points_1_n = points.extract(1,0,
                                             (int)points.nbRows() - 1,
                                             (int)points.nbCols());
      compute_bezier(points_0_n1, t, B_0_n1);
      compute_bezier(points_1_n , t, B_1_n);
      result = (1 - t)*B_0_n1 + t*B_1_n;
      return;
    }

    ml::Vector& Bezier::computeState(ml::Vector& res, int time)
    {
      ml::Matrix controlPoints = controlPointsSIN(time);
      const double t = tSIN(time);
      compute_bezier(controlPoints,
                     t, res);
      return res;

    }



  }
}


