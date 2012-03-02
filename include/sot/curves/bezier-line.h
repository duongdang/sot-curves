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


#ifndef __SOT_BEZIERLINE_H__
#define __SOT_BEZIERLINE_H__
# include <jrl/mal/boost.hh>

namespace dynamicgraph {
  namespace sot {
    namespace curves {
      void compute_bezier(maal::boost::Matrix Points, const float& time,  maal::boost::Vector & res);
    }
  }
}

#endif
