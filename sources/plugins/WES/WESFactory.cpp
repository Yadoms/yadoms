#include "stdafx.h"
#include "WESFactory.h"
#include "equipments/WESEquipment.h"
#include "equipments/tooLowRevisionException.hpp"
#include <shared/Log.h>

CWESFactory::CWESFactory()
{
}

boost::shared_ptr<CIOManager> CWESFactory::loadConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                             const boost::shared_ptr<IWESConfiguration> configuration) const
{
   std::vector<boost::shared_ptr<equipments::IEquipment>> deviceList;
   boost::shared_ptr<equipments::IEquipment> equipment;

   // Create all devices and equipments
   for (const auto& device : api->getAllDevices())
   {
      std::string type = "";

      // plugin state have no type
      try
      {
         type = api->getDeviceDetails(device).getWithDefault<std::string>("type", "");

         if (type == "WES")
         {
            // we create the equipment by sending a frame through the network to each device
            equipment = boost::make_shared<equipments::CWESEquipment>(api,
                                                                      device,
                                                                      api->getDeviceConfiguration(device),
                                                                      configuration);
            deviceList.push_back(equipment);
         }

         // Do Noting, all is done into CWESEquipments
         if (type == "TIC")
         {}
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << e.what();
         throw;
      }

      YADOMS_LOG(information) << "Name : " << device;
      YADOMS_LOG(information) << "Model : " << type;
   }

   return boost::make_shared<CIOManager>(deviceList);
}

std::string CWESFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                              const boost::shared_ptr<CIOManager> ioManager,
                                              const yApi::IManuallyDeviceCreationData& data,
                                              const boost::shared_ptr<IWESConfiguration> configuration) const
{
   boost::shared_ptr<equipments::IEquipment> equipment;

   if (data.getDeviceType() == "WES")
   {
      // TODO : Manage the case where the device is not connected when restart. The device should be created with just the statuts in the plugin
      equipment = boost::make_shared<equipments::CWESEquipment>(api,
                                                                data.getDeviceName(),
                                                                data.getConfiguration(),
                                                                configuration);
      ioManager->addEquipment(equipment);
   }
   else
   {
      YADOMS_LOG(error) << "no section defined for " << data.getDeviceType();
   }

   return equipment->getDeviceName();
}

CWESFactory::~CWESFactory()
{
}

