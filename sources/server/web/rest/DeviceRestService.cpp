#include "stdafx.h"
#include "DeviceRestService.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "json/DeviceEntitySerializer.h"
#include "json/JsonCollectionSerializer.h"

CDeviceRestService::CDeviceRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("device")
{
}


CDeviceRestService::~CDeviceRestService()
{
}

const std::string & CDeviceRestService::getRestKeyword()
{
   return m_restKeyword;
}


CJson CDeviceRestService::readObject(const std::string & objectId)
{
   CDeviceEntitySerializer hes;
   boost::shared_ptr<CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
   return hes.serialize(*deviceFound.get());
}

CJson CDeviceRestService::readObjects()
{
   CDeviceEntitySerializer hes;
   std::vector< boost::shared_ptr<CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevices();
   return CJonCollectionSerializer<CDevice>::SerializeCollection(dvList, hes, getRestKeyword());
}
