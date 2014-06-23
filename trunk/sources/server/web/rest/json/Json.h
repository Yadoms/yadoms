#pragma once
#include <shared/DataContainer.h>


namespace web { namespace rest { namespace json {

   //----------------------------------------
   ///\brief   Define the typename used for Json object
   //----------------------------------------
   typedef shared::CDataContainer CJson;

   //----------------------------------------
   ///\brief   Define a static JSon class serializer
   //----------------------------------------
   class CJsonSerializer
   {
   private:
      CJsonSerializer() {}

      virtual ~CJsonSerializer() {}

   public:
      static shared::CDataContainer deserialize(const std::string& str)
      {
         shared::CDataContainer pt(str);
         return pt;
      }

      static const std::string serialize(const shared::CDataContainer& pt)
      {
         return pt.serialize();
      }

   };

} //namespace json
} //namespace rest
} //namespace web 

