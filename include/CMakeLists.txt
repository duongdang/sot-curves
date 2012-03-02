# Copyright 2012 Duong Dang
#
# This file is part of sot-curves.
# sot-curves is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# sot-curves is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# sot-curves. If not, see <http://www.gnu.org/licenses/>.

FILE(GLOB ALL_HEADERS sot/core/*.h)

INSTALL(FILES ${ALL_HEADERS}
  DESTINATION include/sot/curves
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE
  )
