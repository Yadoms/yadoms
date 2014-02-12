#pragma once

typedef boost::property_tree::ptree CJson;

#include <shared/Log.h>
#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"

template<class TObjectBase>
class CJsonCollectionSerializer
{
public:
   static CJson SerializeCollection(typename std::vector< boost::shared_ptr<TObjectBase> > & collectionToSerialize, IEntitySerializer<TObjectBase> & entitySerilizer, const std::string & itemKeyword)
   {
      CJson objectList;
   
      typename std::vector< boost::shared_ptr<TObjectBase> >::iterator i;
      for(i=collectionToSerialize.begin(); i!=collectionToSerialize.end(); i++)
      {
         objectList.push_back(std::make_pair("", entitySerilizer.serialize(*i->get())));
      }

      if(itemKeyword.empty())
      {
         return objectList;
      }
      else
      {
         CJson result;
         result.push_back(std::make_pair(itemKeyword, objectList));
         return result;
      }
   }


   static typename std::vector< boost::shared_ptr<TObjectBase> > DeserializeCollection(CJson data, IEntityDeserializer<TObjectBase> & entitySerilizer, const std::string & itemKeyword)
   {
      std::vector< boost::shared_ptr<TObjectBase> > result;

      CJson objectList = data;
      if(objectList.find(itemKeyword) != objectList.not_found())
         objectList = objectList.get_child(itemKeyword);

      for (CJson::const_iterator it = objectList.begin(); it != objectList.end(); ++it)
      {
         result.push_back(entitySerilizer.deserialize(it->second));
      }
      return result;
   }
};
