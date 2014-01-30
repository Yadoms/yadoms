#pragma once

#include "IPtreeToStringSerializer.h"

//--------------------------------------------------------------
/// \class Property tree to JSON serializer
//--------------------------------------------------------------
class CPtreeToJsonSerializer : public IPtreeToStringSerializer
{
public:
   virtual ~CPtreeToJsonSerializer();

   // IPtreeToStringSerializer implementation
   virtual const std::string serialize(const boost::property_tree::ptree& pt) const;
   virtual void deserialize(const std::string& str, boost::property_tree::ptree& pt);
   // [END] IPtreeToStringSerializer implementation
};

