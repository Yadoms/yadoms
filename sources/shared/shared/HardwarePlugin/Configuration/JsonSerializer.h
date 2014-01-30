#pragma once

#include "IConfigurationSerializer.h"

//--------------------------------------------------------------
/// \class Hardware plugin configuration JSON serializer
//--------------------------------------------------------------
class CJsonSerializer : public IConfigurationSerializer
{
public:
   virtual ~CJsonSerializer();

   // IConfigurationSerializer implementation
   virtual const std::string serialize(const boost::property_tree::ptree& pt) const;
   virtual void unserialize(const std::string& str, boost::property_tree::ptree& pt);
   // [END] IConfigurationSerializer implementation
};

