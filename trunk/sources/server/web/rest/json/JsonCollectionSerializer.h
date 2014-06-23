#pragma once

#include <shared/Log.h>
#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"

namespace web { namespace rest { namespace json {


   template<class TObjectBase>
   class CJsonCollectionSerializer
   {
   public:
      virtual ~CJsonCollectionSerializer() {}


      static CJson SerializeCollection(typename std::vector< TObjectBase > & collectionToSerialize, const std::string & itemKeyword)
      {
         web::rest::json::CJson objectList;
         objectList.setValues(itemKeyword, collectionToSerialize);
         return objectList;
      }

      

      static CJson SerializeCollection(typename std::vector< boost::shared_ptr<TObjectBase> > & collectionToSerialize, IEntitySerializer<TObjectBase> & entitySerilizer, const std::string & itemKeyword)
      {
         std::vector<shared::CDataContainer> objectList;
         typename std::vector< boost::shared_ptr<TObjectBase> >::iterator i;
         for(i=collectionToSerialize.begin(); i!=collectionToSerialize.end(); i++)
         {
            objectList.push_back(entitySerilizer.serialize(*i->get()));
         }

         CJson result;
         result.setValues(itemKeyword, objectList);
         return result;
      }




      static typename std::vector< boost::shared_ptr<TObjectBase> > DeserializeCollection(CJson data, IEntityDeserializer<TObjectBase> & entitySerilizer, const std::string & itemKeyword)
      {
         std::vector< boost::shared_ptr<TObjectBase> > result;

         std::vector<std::string> temp;
         CJson objectList = data;
         if(objectList.hasValue(itemKeyword))
            temp = objectList.getValues<std::string>(itemKeyword);

         for (std::vector<std::string> ::iterator it = temp.begin(); it != temp.end(); ++it)
         {
            CJson obj(*it);
            result.push_back(entitySerilizer.deserialize(obj));
         }
         return result;
      }
   };


} //namespace json
} //namespace rest
} //namespace web 

