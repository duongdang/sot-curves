#include <cmath>
#include <time.h>
#ifndef WIN32
# include <sys/time.h>
#else
# include <jrl/mal/boost.hh>
# include <sot/core/utils-windows.hh>
# include <Winsock2.h>
#endif /*WIN32*/

#include <sot/curves/nurb.h>
#include <sot/core/debug.hh>
#include <sot/core/macros-signal.hh>
#include <dynamic-graph/factory.h>
#include <fstream>

#include <dynamic-graph/entity.h>

#include <openNURBS/opennurbs.h>


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


    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Nurb,"Nurb");
    Nurb::Nurb( const std::string & name )
      : Entity(name),
        dimension_(3),       // dimension 1, 2 or 3
        bIsRational_(false), // true if rational
        degree_(3),          //
        order_(4),           // order = degree+1
        cv_count_(0),        // number of control vertices
        on_curve_(new ON_NurbsCurve(dimension_, bIsRational_, order_, cv_count_)),
        cv_(),
        knots_(),
        tSIN(NULL,"sotNurb("+name+")::input(double)::t"),
        cvSIN(NULL,"sotNurb("+name+")::input(vector)::cv"),
        knotsSIN(NULL,"sotNurb("+name+")::input(vector)::knots"),
        degreeSIN(NULL,"sotNurb("+name+")::input(vector)::degree"),
        stateSOUT( boost::bind(&Nurb::computeState,this,_1,_2),
                   cvSIN<<tSIN,
                   "sotNurb("+name+")::output(vector)::state" )
    {
      sotDEBUGIN(5);
      signalRegistration( cvSIN<<knotsSIN<<degreeSIN<<tSIN<<stateSOUT);
      sotDEBUGOUT(5);
    }

    ml::Vector& Nurb::computeState(ml::Vector& res, int time)
    {
      ml::Vector cv = cvSIN(time);
      ml::Vector knots = knotsSIN(time);
      int degree = degreeSIN(time);
      const double t = tSIN(time);
      // m + 1 == n + 1 + p + 1
      assert(knots.size() == cv.size()/3 + degree  + 1);
      // If cv or knots has changed, recreate the nurb_curve

      if ( !( check_close(cv, cv_) && check_close(knots, knots_) &&
              degree == degree_
              ) )
        {
          degree_ = degree;
          order_ = degree_ + 1;
          on_curve_->Destroy();
          cv_count_ = cv.size()/3;
          on_curve_->Create(dimension_, bIsRational_, order_,
                            cv_count_);

          for (unsigned i = 0; i < knots.size(); i++)
            {
	      // opennurbs does not count the first and the last knot
	      if (i > 0 && i < knots.size() -1 )
		{
		  on_curve_->SetKnot(i-1, knots(i) );
		}
              if (i < cv_count_)
                {
                  on_curve_->SetCV(i, ON_3dPoint(cv(3*i), cv(3*i+1), cv(3*i+2)) );
                }
            }
        }
      res.resize(3);
      ON_3dPoint p = on_curve_->PointAt(t);

      res(0) = p[0];
      res(1) = p[1];
      res(2) = p[2];

      cv_ = cv;
      knots_ = knots;
      return res;
    }
  }
}


