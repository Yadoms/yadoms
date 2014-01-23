#include "stdafx.h"
#include "WidgetRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/WidgetEntitySerializer.h"
#include "json/JsonCollectionSerializer.h"

CWidgetRestService::CWidgetRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("widget")
{
}


CWidgetRestService::~CWidgetRestService()
{
}

const std::string & CWidgetRestService::getRestKeyword()
{
   return m_restKeyword;
}


CJson CWidgetRestService::readObject(const std::string & objectId)
{
   CWidgetEntitySerializer hes;
   boost::shared_ptr<CWidget> widgetFound =  m_dataProvider->getWidgetRequester()->getWidget(boost::lexical_cast<int>(objectId));
   return hes.serialize(*widgetFound.get());
}

CJson CWidgetRestService::readObjects()
{
   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > hwList = m_dataProvider->getWidgetRequester()->getWidgets();
   return CJonCollectionSerializer<CWidget>::SerializeCollection(hwList, hes, getRestKeyword());
}
