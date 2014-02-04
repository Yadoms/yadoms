#include "stdafx.h"
#include "DeviceRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"

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

void CDeviceRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CDeviceRestService::getAllDevices);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CDeviceRestService::getOneDevice);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("acquisition"), CDeviceRestService::getDeviceLastAcquisition);
}


CJson CDeviceRestService::getOneDevice(const std::vector<std::string> & parameters)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   CDeviceEntitySerializer hes;
   boost::shared_ptr<CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
   return hes.serialize(*deviceFound.get());
}

CJson CDeviceRestService::getAllDevices(const std::vector<std::string> & parameters)
{
   CDeviceEntitySerializer hes;
   std::vector< boost::shared_ptr<CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevices();
   return CJonCollectionSerializer<CDevice>::SerializeCollection(dvList, hes, getRestKeyword());
}

CJson CDeviceRestService::getDeviceLastAcquisition(const std::vector<std::string> & parameters)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   boost::shared_ptr<CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
   if(deviceFound.get() != NULL)
   {
      CAcquisitionEntitySerializer hes;
      std::vector< boost::shared_ptr<CAcquisition> > allAcq =  m_dataProvider->getAcquisitionRequester()->getLastAcquisitions(deviceFound->getDataSource());
     return CJonCollectionSerializer<CAcquisition>::SerializeCollection(allAcq, hes, "acquisition");
   }
   else
      throw CException("Device not found");
}