#pragma once

#include <shared/serialization/PTreeToJsonSerializer.h>

namespace web { namespace rest { namespace json {

   //----------------------------------------
   ///\brief   Define the typename used for Json object
   //----------------------------------------
   typedef boost::property_tree::ptree CJson;

   //----------------------------------------
   ///\brief   Define a static JSon class serializer
   //----------------------------------------
   class CJsonSerializer
   {
   private:
      CJsonSerializer() {}

      virtual ~CJsonSerializer() {}

   private:
      static boost::shared_ptr<shared::serialization::IPtreeToStringSerializer> createSerializer()
      {
         boost::shared_ptr<shared::serialization::IPtreeToStringSerializer> serializer(new shared::serialization::CPtreeToJsonSerializer);
         return serializer;
      }

   public:
      static boost::property_tree::ptree deserialize(const std::string& str)
      {
         boost::property_tree::ptree pt;
         createSerializer()->deserialize(str, pt);
         return pt;
      }

      static const std::string serialize(const boost::property_tree::ptree& pt)
      {
         return createSerializer()->serialize(pt);
      }

   };

} //namespace json
} //namespace rest
} //namespace web 

