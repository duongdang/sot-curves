#include <cmath>
#include <time.h>
#ifndef WIN32
# include <sys/time.h>
#else
# include <jrl/mal/boost.hh>
# include <sot/core/utils-windows.hh>
# include <Winsock2.h>
#endif /*WIN32*/


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
    class Bezier
  : public Entity
{
public:
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }
  Bezier(const std::string &);
};

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Bezier,"Bezier");

Bezier::Bezier( const std::string & name )
  : Entity(name)
    {
      sotDEBUGIN(5);
      sotDEBUGOUT(5);
    }

  }
}


