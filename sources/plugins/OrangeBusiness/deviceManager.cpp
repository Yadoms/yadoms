#include "stdafx.h"
#include "deviceManager.h"
#include "DefaultEquipment.h"
#include <shared/DataContainer.h>
#include <shared/http/HttpException.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

CEquipmentManager::CEquipmentManager(const std::map<std::string, boost::shared_ptr<equipments::IEquipment>>& deviceList) :
	m_deviceList (deviceList)
{}

CEquipmentManager::CEquipmentManager(boost::shared_ptr<yApi::IYPluginApi> api)
{
	std::vector<std::string> devices = api->getAllDevices();

	for (const auto& device : devices)
	{
		try  // plugin state have no type
		{
         std::string devEUI = api->getDeviceDetails(device).get<std::string>("devEUI");

         if (devEUI != "") // Create the device 
            m_deviceList.insert(std::pair<std::string, boost::shared_ptr<equipments::IEquipment>>(device, boost::make_shared<equipments::CDefaultEquipment>(device, devEUI)));
		}
		catch (...)
		{}
	}
}

const int CEquipmentManager::size() const
{
	return m_deviceList.size();
}

void CEquipmentManager::refreshEquipments(boost::shared_ptr<yApi::IYPluginApi> api,
                                          boost::shared_ptr<IurlManager> frameManager,
                                          const std::string& apikey, 
                                          boost::shared_ptr<CDecoder> decoder)
{
	for (const auto& pair : m_deviceList)
	{
      refreshEquipment(api, pair.second, frameManager, apikey, decoder);
	}
}

void CEquipmentManager::refreshEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const boost::shared_ptr<equipments::IEquipment> equipment,
                                         boost::shared_ptr<IurlManager> frameManager,
                                         const std::string& apikey,
                                         boost::shared_ptr<CDecoder> decoder)
{
   boost::posix_time::ptime actualTime = shared::currentTime::Provider().now();

   try {
      // reading of the battery level
      shared::CDataContainer response = frameManager->getDeviceInformation(apikey, equipment->getEUI());
      response.printToLog(YADOMS_LOG(trace));
      std::string receivedTimeString = response.get<std::string>("updateTs");
      boost::remove_erase_if(receivedTimeString, boost::is_any_of("Z-:."));
      boost::posix_time::ptime receivedTime = boost::posix_time::from_iso_string(receivedTimeString);

      boost::posix_time::time_duration elapseTimeSinceLastBatteryMessage = actualTime - receivedTime;
      const boost::posix_time::time_duration maxTimeForBatteryHistorization = boost::posix_time::hours(168);

      // Reading of the last communication date. If the date is too old for battery level > 1 week - do not historize it
      try {
         if (elapseTimeSinceLastBatteryMessage >= maxTimeForBatteryHistorization)
         {
            equipment->updateBatteryLevel(api, response.get<int>("lastBatteryLevel"));
         }
      }
      catch (std::exception &)
      {
         YADOMS_LOG(information) << "no battery information for equipment " << equipment->getName();
      }
   }
   catch (shared::CHttpException &e)
   {
      throw e;
   }
   catch (std::exception &e)
   {
      YADOMS_LOG(error) << "exception 1 : " << e.what();
   }

   try {
      // Reading all commands for an equipment. lastest messages should be write first
      shared::CDataContainer response = frameManager->listDeviceCommands(apikey, equipment->getEUI(), 0);
      response.printToLog(YADOMS_LOG(trace));

      // get last data from this last frame
      shared::CDataContainer lastData = decoder->getLastData(response);
      response.printToLog(YADOMS_LOG(trace));

      std::string idNewMessage = lastData.getWithDefault<std::string>("id", "");
      std::string idlastMessage = equipment->getlastMessageId(api);

      // Register the new data, only if it's a new id
      // Should be enhanced in the futur
      if (idNewMessage != idlastMessage)
      {
         std::string receivedTimeString = lastData.get<std::string>("timestamp");
         boost::remove_erase_if(receivedTimeString, boost::is_any_of("Z-:."));
         boost::posix_time::ptime receivedTime = boost::posix_time::from_iso_string(receivedTimeString);
         boost::posix_time::time_duration elapseTimeSinceLastDataMessage = actualTime - receivedTime;
         const boost::posix_time::time_duration maxTimeForDataHistorization = boost::posix_time::minutes(30);
         equipment->updateData(api, lastData.get<std::string>("payload"));
         equipment->updatelastMessageId(api, idNewMessage);
      }
   }
   catch (shared::CHttpException &e)
   {
      throw e;
   }
   catch (std::exception &e)
   {
      YADOMS_LOG(error) << "exception 2 : " << e.what();
   }
}

void CEquipmentManager::removeAllDevices(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Delete all devices created in Yadoms";

   for (const auto& pair : m_deviceList)
      pair.second->removeFromYadoms(api);

   m_deviceList.clear();

}

CEquipmentManager::~CEquipmentManager()
{}