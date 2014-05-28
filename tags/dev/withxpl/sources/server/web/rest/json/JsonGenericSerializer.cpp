#include "stdafx.h"

#include "JsonGenericSerializer.h"

namespace web { namespace rest { namespace json {

   CJsonGenericSerializer::CJsonGenericSerializer()
   {
   }

   CJsonGenericSerializer::~CJsonGenericSerializer()
   {
   }

   CJson CJsonGenericSerializer::serialize(const std::vector<std::map<std::string, std::string> > & data)
   {
      CJson objectList;

      std::vector<std::map<std::string, std::string> >::const_iterator i;

      for(i=data.begin(); i!=data.end(); ++i)
      {
         CJson result;

         std::map<std::string, std::string>::const_iterator fieldIterator;
         for(fieldIterator = i->begin(); fieldIterator != i->end(); ++fieldIterator)
         {
            result.put(fieldIterator->first, fieldIterator->second);
         }
         objectList.push_back(std::make_pair("", result));
      }
      return objectList;
   }



} //namespace json
} //namespace rest
} //namespace web 

