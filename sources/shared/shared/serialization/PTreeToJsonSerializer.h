#pragma once

#include <shared/Export.h>

#include "IPtreeToStringSerializer.h"

namespace shared { namespace serialization
{

   //--------------------------------------------------------------
   /// \class Property tree to JSON serializer
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CPtreeToJsonSerializer : public IPtreeToStringSerializer
   {
   public:
      virtual ~CPtreeToJsonSerializer();

      // IPtreeToStringSerializer implementation
      virtual const std::string serialize(const boost::property_tree::ptree& pt) const;
      virtual void deserialize(const std::string& str, boost::property_tree::ptree& pt);
      // [END] IPtreeToStringSerializer implementation
   };

} } // namespace shared::serialization
