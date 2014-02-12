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


   static void print(boost::property_tree::ptree const& pt)
   {
      using boost::property_tree::ptree;
      ptree::const_iterator end = pt.end();
      for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
         YADOMS_LOG(info) << it->first << ": " << it->second.get_value<std::string>();
         print(it->second);
      }
   }


   static typename std::vector< boost::shared_ptr<TObjectBase> > DeserializeCollection(CJson data, IEntityDeserializer<TObjectBase> & entitySerilizer, const std::string & itemKeyword)
   {
      std::vector< boost::shared_ptr<TObjectBase> > result;

      if(data.find(itemKeyword) != data.not_found())
      {
         CJson objectList = data.get_child(itemKeyword);
         for (CJson::const_iterator it = objectList.begin(); it != objectList.end(); ++it)
         {
            result.push_back(entitySerilizer.deserialize(it->second));
         }
      }
      else
      {
         CJson objectList = data;
         CJsonSerializer a;
         for (CJson::const_iterator it = objectList.begin(); it != objectList.end(); ++it)
         {
            CJson currentNode;
            a.deserialize(it->second.data(), currentNode);
            result.push_back(entitySerilizer.deserialize(currentNode));
         }
      }


      YADOMS_LOG(info) << "Print input";
      print(data);

      CJson objectList = data;
      if(objectList.find(itemKeyword) != objectList.not_found())
         objectList = objectList.get_child(itemKeyword);

      YADOMS_LOG(info) << "Print usefull";
      print(objectList);

      for (CJson::const_iterator it = objectList.begin(); it != objectList.end(); ++it)
      {
         YADOMS_LOG(info) << "Print item";
         print(it->second);
         result.push_back(entitySerilizer.deserialize(it->second));
      }
      return result;
   }
};
