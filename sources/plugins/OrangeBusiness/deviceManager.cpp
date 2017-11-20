#include "stdafx.h"
#include "deviceManager.h"
#include "DefaultEquipment.h"
#include <shared/DataContainer.h>

CEquipmentManager::CEquipmentManager(const std::map<std::string, boost::shared_ptr<equipments::IEquipment>>& deviceList) :
	m_deviceList (deviceList)
{}

CEquipmentManager::CEquipmentManager(boost::shared_ptr<yApi::IYPluginApi> api)
{
	std::vector<std::string> devices = api->getAllDevices();

	// Create all extensions devices
	for (const auto& device : devices)
	{
		std::string devEUI = "";

		// plugin state have no type
		try
		{
			devEUI = api->getDeviceDetails(device).get<std::string>("devEUI");
		}
		catch (...)
		{
		}

		if (devEUI != "") // Create the device 
			m_deviceList.insert(std::pair<std::string, boost::shared_ptr<equipments::IEquipment>>(device, boost::make_shared<equipments::CDefaultEquipment>(device, devEUI)));
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
		int page = 0;

		try {
			// reading of the battery level
			shared::CDataContainer response = frameManager->getDeviceInformation(apikey, pair.second->getEUI());
			response.printToLog(YADOMS_LOG(information));
			boost::posix_time::ptime receivedTime(response.get<boost::posix_time::ptime>("updateTs"));
         boost::posix_time::ptime actualTime = shared::currentTime::Provider().now();
			boost::posix_time::time_duration elapseTimeSinceLastBatteryMessage = actualTime - receivedTime;
         const boost::posix_time::time_duration maxTimeForBatteryHistorization = boost::posix_time::hours(168);

			// Reading of the last communication date. If the date is too old for battery level > 1 week - do not historize it
			if (elapseTimeSinceLastBatteryMessage < maxTimeForBatteryHistorization)
			{
            m_deviceList.at(pair.first)->updateBatteryLevel(api, response.get<int>("lastBatteryLevel"));
			}

			// Reading all commands for an equipment. lastest messages should be write first
         response = frameManager->listDeviceCommands(apikey, pair.second->getEUI(), 0);

			// get last data from this last frame
			response = decoder->getLastData(response);

         // Register the new data, only if it's a new id
         if (response.get<std::string>("id") != m_deviceList.at(pair.first)->getlastMessageId(api))
         {
            boost::posix_time::ptime receivedTime(response.get<boost::posix_time::ptime>("updateTs"));
            boost::posix_time::time_duration elapseTimeSinceLastDataMessage = actualTime - receivedTime;
            const boost::posix_time::time_duration maxTimeForDataHistorization = boost::posix_time::minutes(30);

            // If the time is > 30mn, we do not historize it
            if (elapseTimeSinceLastDataMessage < maxTimeForDataHistorization)
            {
               m_deviceList.at(pair.first)->updateData(api, response.get<std::string>("data"));
            }
         }
		}
		catch (std::exception &e)
		{
			YADOMS_LOG(error) << "exception : " << e.what();
		}
	}
}

CEquipmentManager::~CEquipmentManager()
{}