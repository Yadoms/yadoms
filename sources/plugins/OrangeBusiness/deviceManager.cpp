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
			int batteryLevel = response.get<int>("lastBatteryLevel");
			boost::posix_time::ptime receivedTime(response.get<boost::posix_time::ptime>("updateTs"));
			boost::posix_time::time_duration td = shared::currentTime::Provider().now() - receivedTime;

			// Todo : Reading of the last communication date. If the date is too old for battery level > 1 week - do not integrate it
			if (td.total_seconds() < (3600 * 24 * 7)) // A améliorer
			{
            m_deviceList.at(pair.first)->updateBatteryLevel(api, batteryLevel);
			}

			//Todo : Enter a date to limit the number of frames
			// Reading all commands for an equipment. Go to the last page if necessary
			do {
				response = frameManager->listDeviceCommands(apikey, pair.second->getEUI(), page);
				response.printToLog(YADOMS_LOG(information));
				++page;
			} while (!decoder->isFrameComplete(response));

			// get last data from this last frame
			response = decoder->getLastData(response);
         m_deviceList.at(pair.first)->updateData(api, response);
			// Todo : Reading of the last communication date. If the date is too old for data > 15mn - do not integrate it

		}
		catch (std::exception &e)
		{
			YADOMS_LOG(error) << "exception : " << e.what();
		}
	}
}

CEquipmentManager::~CEquipmentManager()
{}