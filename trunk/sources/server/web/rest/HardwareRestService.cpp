#include "stdafx.h"
#include "HardwareRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/HardwareEntitySerializer.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "shared/Log.h"


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

void CHardwareRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CHardwareRestService::getAllHardwares);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CHardwareRestService::getOneHardware);
}

 CJson CHardwareRestService::getOneHardware(const std::vector<std::string> & parameters)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   CHardwareEntitySerializer hes;
   boost::shared_ptr<CHardware> hardwareFound =  m_dataProvider->getHardwareRequester()->getHardware(boost::lexical_cast<int>(objectId));
   return hes.serialize(*hardwareFound.get());
}

CJson CHardwareRestService::getAllHardwares(const std::vector<std::string> & parameters)
{
   CHardwareEntitySerializer hes;
   std::vector< boost::shared_ptr<CHardware> > hwList = m_dataProvider->getHardwareRequester()->getHardwares();
   return CJonCollectionSerializer<CHardware>::SerializeCollection(hwList, hes, getRestKeyword());
}
