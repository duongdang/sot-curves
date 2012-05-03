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


#ifndef __SOT_INTERPOLATOR_H__
#define __SOT_INTERPOLATOR_H__
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
    class Interpolator
  : public Entity
{
 public:
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }
  Interpolator(const std::string &);

  dg::SignalPtr<int, int> dimensionSIN;
  dg::SignalPtr<ml::Vector, int> stampsSIN;
  dg::SignalPtr<ml::Vector, int> v0SIN;
  dg::SignalPtr<ml::Vector, int> vnSIN;
  dg::SignalPtr<ml::Vector, int> qsSIN;

  dg::SignalTimeDependent<ml::Vector ,int> knotsSOUT;
  dg::SignalTimeDependent<ml::Vector ,int> cvSOUT;

  ml::Vector& compute_knots(ml::Vector& res, int time);
  ml::Vector& compute_control_points(ml::Vector& res, int time);


 private:
  int dimension_;
  ml::Vector stamps_;
  ml::Vector v0_;
  ml::Vector vn_;
  ml::Vector qs_;
  ml::Vector knots_;
  ml::Vector normed_knots_;
  ml::Vector cv_;
};


  }
}


#endif
