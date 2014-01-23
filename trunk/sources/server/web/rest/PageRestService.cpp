#include "stdafx.h"
#include "PageRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/PageEntitySerializer.h"
#include "json/JsonCollectionSerializer.h"

CPageRestService::CPageRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("page")
{
}


CPageRestService::~CPageRestService()
{
}

const std::string & CPageRestService::getRestKeyword()
{
   return m_restKeyword;
}


CJson CPageRestService::readObject(const std::string & objectId)
{
   CPageEntitySerializer hes;
   boost::shared_ptr<CPage> pageFound =  m_dataProvider->getPageRequester()->getPage(boost::lexical_cast<int>(objectId));
   return hes.serialize(*pageFound.get());
}

CJson CPageRestService::readObjects()
{
   CPageEntitySerializer hes;
   std::vector< boost::shared_ptr<CPage> > hwList = m_dataProvider->getPageRequester()->getPages();
   return CJonCollectionSerializer<CPage>::SerializeCollection(hwList, hes, getRestKeyword());
}
