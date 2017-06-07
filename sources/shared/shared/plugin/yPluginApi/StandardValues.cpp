#include "stdafx.h"
#include "StandardCapacities.h"
#include "StandardUnits.h"
#include "StandardValues.h"


#define DECLARE_VALUE(cppName, name) \
   const std::string& CStandardValues::cppName() \
   { \
      static const std::string value(name); \
      return value; \
   }

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         DECLARE_VALUE(On, "on");
         DECLARE_VALUE(Off, "off");
         DECLARE_VALUE(Dim, "dim");
         DECLARE_VALUE(Open, "open");
         DECLARE_VALUE(Close, "close");
         DECLARE_VALUE(Stop, "stop");
      }
   }
} // namespace shared::plugin::yPluginApi


