#include "stdafx.h"
#include "HardwareRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/HardwareEntitySerializer.h"
#include "json/JsonCollectionSerializer.h"

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
   CHardwareEntitySerializer hes;
   std::vector< boost::shared_ptr<CHardware> > hwList = m_dataProvider->getHardwareRequester()->getHardwares();
   return CJonCollectionSerializer<CHardware>::SerializeCollection(hwList, hes, getRestKeyword());
}
