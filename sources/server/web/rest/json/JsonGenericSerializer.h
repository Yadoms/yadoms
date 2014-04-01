#pragma once

#include "Json.h"

namespace web { namespace rest { namespace json {

   class CJsonGenericSerializer
   {
   public:
      CJsonGenericSerializer();
      virtual ~CJsonGenericSerializer();

      CJson serialize(const std::vector<std::map<std::string, std::string> > & object);
   };

} //namespace json
} //namespace rest
} //namespace web 

