#include "stdafx.h"
#include "HardwareRestService.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "json/HardwareEntitySerializer.h"

CHardwareRestService::CHardwareRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("hardware")
{
}


CHardwareRestService::~CHardwareRestService()
{
}

const std::string & CHardwareRestService::getRestKeyword()
{
   return m_restKeyword;
}


CJson CHardwareRestService::readObject(const std::string & objectId)
{
   CHardwareEntitySerializer hes;
   boost::shared_ptr<CHardware> hardwareFound =  m_dataProvider->getHardwareRequester()->getHardware(boost::lexical_cast<int>(objectId));
   return hes.serialize(*hardwareFound.get());
}

CJson CHardwareRestService::readObjects()
{
   CJson result;
   CJson objectList;
   CHardwareEntitySerializer hes;
   std::vector<boost::shared_ptr<CHardware> > hardwares =  m_dataProvider->getHardwareRequester()->getHardwares();
   std::vector<boost::shared_ptr<CHardware> >::iterator i;
   for(i=hardwares.begin(); i!=hardwares.end(); i++)
   {
      CHardware * p = i->get();
      objectList.push_back(std::make_pair("", hes.serialize(*p)));
   }
   result.push_back(std::make_pair(getRestKeyword(), objectList));
   return result;
}
