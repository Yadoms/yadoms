#include "stdafx.h"
#include "DeviceRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "json/JsonResult.h"
#include "RestDispatcherHelpers.hpp"
#include "AcquisitionRestService.h"

std::string CDeviceRestService::m_restKeyword= std::string("device");


CDeviceRestService::CDeviceRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
   :m_dataProvider(dataProvider)
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
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")(CAcquisitionRestService::getRestKeyword()), CDeviceRestService::getDeviceLastAcquisition);
}


CJson CDeviceRestService::getOneDevice(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   CDeviceEntitySerializer hes;
   boost::shared_ptr<database::entities::CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
   return CJsonResult::GenerateSuccess(hes.serialize(*deviceFound.get()));
}

CJson CDeviceRestService::getAllDevices(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CDeviceEntitySerializer hes;
   std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevices();
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<database::entities::CDevice>::SerializeCollection(dvList, hes, getRestKeyword()));
}

CJson CDeviceRestService::getDeviceLastAcquisition(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   boost::shared_ptr<database::entities::CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
   if(deviceFound.get() != NULL)
   {
      CAcquisitionEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CAcquisition> > allAcq =  m_dataProvider->getAcquisitionRequester()->getLastAcquisitions(deviceFound->getDataSource());
     return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<database::entities::CAcquisition>::SerializeCollection(allAcq, hes, CAcquisitionRestService::getRestKeyword()));
   }
   else
      return CJsonResult::GenerateError("Device not found");
}