#pragma once

#include "IPtreeToStringSerializer.h"

//--------------------------------------------------------------
/// \class Property tree to JSON serializer
//--------------------------------------------------------------
class PtreeToJsonSerializer : public IPtreeToStringSerializer
{
public:
   virtual ~PtreeToJsonSerializer();

   // IPtreeToStringSerializer implementation
   virtual const std::string serialize(const boost::property_tree::ptree& pt) const;
   virtual void unserialize(const std::string& str, boost::property_tree::ptree& pt);
   // [END] IPtreeToStringSerializer implementation
};

