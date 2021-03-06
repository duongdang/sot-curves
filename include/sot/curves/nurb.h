// Copyright 2012 Duong Dang
//
// This file is part of sot-curves.
// sot-curves is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// sot-curves is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// sot-curves. If not, see <http://www.gnu.org/licenses/>.


#ifndef __SOT_NURB_H__
#define __SOT_NURB_H__
# include <jrl/mal/boost.hh>
namespace ml = maal::boost;

/* SOT */
#include <sot/core/flags.hh>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <sot/core/exception-dynamic.hh>
#include <sot/core/matrix-homogeneous.hh>
#include <openNURBS/opennurbs.h>

namespace dynamicgraph
{
  namespace sot
  {
    namespace dg = dynamicgraph;
    class Nurb
  : public Entity
{
 public:
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }
  Nurb(const std::string &);

  dg::SignalPtr<double, int> tSIN;
  dg::SignalPtr<ml::Vector, int> cvSIN;
  dg::SignalPtr<ml::Vector, int> knotsSIN;
  dg::SignalPtr<int, int> degreeSIN;

  dg::SignalTimeDependent<ml::Vector ,int> stateSOUT;
  ml::Vector& computeState(ml::Vector& res, int time);


  void compute_nurb(maal::boost::Matrix Points, const double& t,
                      maal::boost::Vector & res);

 private:
  int dimension_;
  ON_BOOL32 bIsRational_;
  int degree_;
  int order_;
  int cv_count_;
  ON_NurbsCurve* on_curve_;
  ml::Vector cv_;
  ml::Vector knots_;

};


  }
}


#endif
