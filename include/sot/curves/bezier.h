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


#ifndef __SOT_BEZIER_H__
#define __SOT_BEZIER_H__
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

namespace dynamicgraph
{
  namespace sot
  {
    namespace dg = dynamicgraph;
    class Bezier
  : public Entity
{
public:
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }
  Bezier(const std::string &);

  dg::SignalPtr<double, int> tSIN;
  dg::SignalPtr<ml::Matrix, int> controlPointsSIN;

  dg::SignalTimeDependent<ml::Vector ,int> stateSOUT;

  ml::Vector& computeState(ml::Vector& res, int time);


  void compute_bezier(maal::boost::Matrix Points, const double& t,
                      maal::boost::Vector & res);

};


  }
}


#endif
