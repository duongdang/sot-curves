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


#ifndef __SOT_TIMELINE_H__
#define __SOT_TIMELINE_H__
# include <jrl/mal/boost.hh>
namespace ml = maal::boost;

/* SOT */
#include <sot/core/flags.hh>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <sot/core/exception-dynamic.hh>

namespace dynamicgraph
{
  namespace sot
  {
    namespace dg = dynamicgraph;
    class Timeline
  : public Entity
{
public:
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }
  Timeline(const std::string &);

 public: // Trigger

  /// Trigger signal, to be updated periodically to trigger a
  /// call to the synchronization method. Typically, this signal
  /// is added to the periodic calls of OpenHRP.
  Signal< int,int > triggerSOUT;

  /// The trigger callback function, which implements the synchronization
  /// of all the sub-components used to generate and send the steps.
  int& triggerCall( int& dummy, int timeCurr );
  double& compute( double& res, int time );

  Signal< double,int > timeMaxSIN;
  Signal< double,int > scaledTimeSOUT;


 private:

  enum State
  {
    START,
    STOP
  };

  void setState(const std::string& statearg);

  State state_;
  unsigned count_;
  double period_;

};


  }
}


#endif
