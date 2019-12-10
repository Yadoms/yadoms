#include "stdafx.h"
#include "VRTIPFactory.h"
#include "equipments/VRTIPEquipment.h"
#include "http/timeOutException.hpp"
#include <shared/Log.h>

CVRTIPFactory::CVRTIPFactory()
{
}

boost::shared_ptr<CIOManager> CVRTIPFactory::loadConfiguration(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   std::vector<boost::shared_ptr<equipments::IEquipment>> deviceList;
   std::vector<std::string> deviceToRetry;
   CVRTIPFactory factory;

   // Create all devices and equipments
   for (const auto& device : api->getAllDevices()){
      std::string type = "";

      // plugin state have no type
      try{
         type = api->getDeviceDetails(device).getWithDefault<std::string>("type", "");

         if (type == "WES"){
            try{
               deviceList.push_back(factory.createEquipment(api, device));
            }
            catch (CTimeOutException&){
               deviceToRetry.push_back(device);
            }
            catch (std::exception&)
            {
				//TODO : Try restart the plugin with the equipment connected => We should have a error
            }
         }

         // Do Noting, all is done into CWESEquipments
         if (type == "TIC")
         {}
      }
      catch (std::exception& e){
         YADOMS_LOG(error) << e.what();
         throw;
      }

      YADOMS_LOG(information) << "Name : " << device;
      YADOMS_LOG(information) << "Model : " << type;
   }

   return boost::make_shared<CIOManager>(deviceList, deviceToRetry);
}

boost::shared_ptr<equipments::IEquipment> CVRTIPFactory::createEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                         const std::string& device)
{
   // we create the equipment by sending a frame through the network to each device
      return boost::make_shared<equipments::CVRTIPEquipment>(api,
                                                             device,
                                                             api->getDeviceConfiguration(device));
}

std::string CVRTIPFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                                const boost::shared_ptr<CIOManager> ioManager,
                                                const yApi::IManuallyDeviceCreationData& data) const
{
   boost::shared_ptr<equipments::IEquipment> equipment;

   if (data.getDeviceType() == "WES")
   {
      // TODO : Manage the case where the device is not connected when restart. The device should be created with just the statuts in the plugin
      equipment = boost::make_shared<equipments::CVRTIPEquipment>(api,
                                                                  data.getDeviceName(),
                                                                  data.getConfiguration());
      ioManager->addEquipment(equipment);
   }
   else
   {
      YADOMS_LOG(error) << "no section defined for " << data.getDeviceType();
   }

   return equipment->getDeviceName();
}

CVRTIPFactory::~CVRTIPFactory()
{
}

