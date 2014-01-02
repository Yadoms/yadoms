#pragma once

typedef boost::property_tree::ptree CJson;

template<class TObjectBase>
class CJonCollectionSerializer
{
public:
   static CJson SerializeCollection(std::vector<boost::shared_ptr<TObjectBase>> & collectionToSerialize, IEntitySerializer<TObjectBase> & entitySerilizer, const std::string & itemKeyword)
   {
      CJson result;
      CJson objectList;
   
      std::vector<boost::shared_ptr<TObjectBase> >::iterator i;
      for(i=collectionToSerialize.begin(); i!=collectionToSerialize.end(); i++)
      {
         objectList.push_back(std::make_pair("", entitySerilizer.serialize(*i->get())));
      }
      result.push_back(std::make_pair(itemKeyword, objectList));
      return result;

   }
};