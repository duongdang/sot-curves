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
#include <sot/curves/gnurbsutil.h>
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
      update(time);
      res = knots_;
      return res;
    }

    ml::Vector& Interpolator::compute_control_points(ml::Vector& res, int time)
    {
      update(time);
      res = cv_;
      return res;
    }

    void Interpolator::update(int time)
    {
      ml::Vector stamps = stampsSIN(time);
      ml::Vector v0 = v0SIN(time);
      ml::Vector vn = vnSIN(time);
      ml::Vector qs = qsSIN(time);
      int dimension = dimensionSIN(time);
      int p = dimension; // degree
      int n = stamps.size() - 1; // Control points Q0, Q1, ..., Qn
      // with end derivatives
      int m = n + p + 3; // page 370, nurbs book, m + 1: knot count
      // wo end derivatives
      //int m = n + p + 1; // page 370, nurbs book, m + 1: knot count
      assert( 3*stamps.size() == qs.size() );

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
          ml::Vector u_bar = stamps*(1/(stamps(stamps.size()-1)
                                        - stamps(0)));
          // std::cout << u_bar << std::endl;
          normed_knots_.resize(m+1);
          for (unsigned i = 0; i <=p; i++)
            {
              normed_knots_(i) = 0;
              normed_knots_(m-i) = 1;
            }

          for (unsigned j = 0; j <= n - p + 1 ; j++)
            {
              normed_knots_(j+p+1) = 0;
              for (unsigned i = j; i <= j+p-1 ; i++)
                {
                  normed_knots_(j+p+1) += u_bar(i);
                }
              normed_knots_(j+p+1) *= 1.0/p;
            }
          // std::cout << "normed_knots_" << normed_knots_ << std::endl;
          // algo A9.1 nurbs book
          ml::Matrix A(n+3, n+3);
          A.setZero();
          float* U = (float*) malloc(sizeof(float)*normed_knots_.size());
          for (unsigned i = 0; i < normed_knots_.size(); i++)
            {
              U[i] = normed_knots_(i);
            }

          for (unsigned i = 0; i <= n; i++)
            {

              int span = FindSpan(n+2, p, u_bar(i), U);
              float* non_vanish_terms = (float*)malloc (sizeof (float)
                                                        * (p+1));
              BasisFuns(span, u_bar(i), p, U, non_vanish_terms);
              for (unsigned j = span-p; j <= span; j ++)
                {
                  A(i,j) = non_vanish_terms[j - (span-p)];
                  // std::cout << i << " " << j << ":"
                  //          << non_vanish_terms[j - (span-p)] << " ";;
                }
              // std::cout << std::endl;
            }
          A(n+1, 0) = -1;
          A(n+1, 1) = 1;
          A(n+2, n+1) = -1;
          A(n+2, n+2) = 1;
          // std::cout << A << std::endl;

          // AP = R
          // right hand side vector
          ml::Matrix R(n+3, 3);
          for (unsigned i = 0; i < n + 1; i++)
            {
              R(i,0) = qs(3*i + 0);
              R(i,1) = qs(3*i + 1);
              R(i,2) = qs(3*i + 2);

            }
          R(n+1, 0) = 1.0*v0(0)*normed_knots_(p+1)/p;
          R(n+1, 1) = 1.0*v0(1)*normed_knots_(p+1)/p;
          R(n+1, 2) = 1.0*v0(2)*normed_knots_(p+1)/p;
          R(n+2, 0) = 1.0*vn(0)*(1 - normed_knots_(m-p-1))/p;
          R(n+2, 1) = 1.0*vn(1)*(1 - normed_knots_(m-p-1))/p;
          R(n+2, 2) = 1.0*vn(2)*(1 - normed_knots_(m-p-1))/p;
          // std::cout << R << std::endl;


          ml::Matrix Pm = A.inverse()*R;
          cv_.resize(3*(n+3));

          for (unsigned i = 0; i < cv_.size()/3; i++)
            {
              cv_(3*i) = Pm(i,0);
              cv_(3*i+1) = Pm(i,1);
              cv_(3*i+2) = Pm(i,2);
            }
          // std::cout << "cv:" << cv_ << std::endl;
          knots_ = normed_knots_*(stamps(stamps.size()-1) - stamps(0));
          // std::cout << "knots:" << knots_ << std::endl;
        }
    }

  }
}


