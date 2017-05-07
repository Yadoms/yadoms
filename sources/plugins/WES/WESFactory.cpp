#include "stdafx.h"
#include "WESFactory.h"
#include "equipments/WESEquipment.h"
#include "equipments/temperatureProbe.h"
#include "equipments/manuallyDeviceCreationException.hpp"
#include <shared/Log.h>

CWESFactory::CWESFactory()
{}

boost::shared_ptr<CIOManager> CWESFactory::loadConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                             const boost::shared_ptr<IWESConfiguration> configuration)
{
   std::vector<boost::shared_ptr<equipments::IEquipment> > masterdeviceList;
   std::vector<boost::shared_ptr<equipments::IEquipment> > deviceList;

   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   boost::shared_ptr<equipments::IEquipment> equipment;

   // Create all devices and equipments
   for (devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
      std::string type = "";

      // plugin state have no type
      try {
         api->getDeviceDetails(*devicesIterator).printToLog(YADOMS_LOG(information));
         type = api->getDeviceDetails(*devicesIterator).getWithDefault<std::string>("type", "");

         if (type =="WES")
         {
            equipment = boost::make_shared<equipments::CWESEquipment>(api, 
                                                                      (*devicesIterator), 
                                                                      api->getDeviceConfiguration(*devicesIterator));
            masterdeviceList.push_back(equipment);
            deviceList.push_back(equipment);
         }

         if (type == "temperatureProbe")
         {
            equipment = boost::make_shared<equipments::CtemperatureProbe>(api, (*devicesIterator), api->getDeviceConfiguration(*devicesIterator), configuration);
            deviceList.push_back(equipment);
         }

         // Do Noting, all is done into CWESEquipments
         if (type =="TIC")
         {}
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << e.what();
         throw;
      }

      YADOMS_LOG(information) << "Name : " << (*devicesIterator);
      YADOMS_LOG(information) << "Model : " << type;
   }

   return boost::make_shared<CIOManager>(deviceList, masterdeviceList);
}

std::string CWESFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                              const boost::shared_ptr<CIOManager> ioManager,
                                              const yApi::IManuallyDeviceCreationData& data,
                                              const boost::shared_ptr<IWESConfiguration> configuration)
{
   boost::shared_ptr<equipments::IEquipment> equipment;

   try {

      data.getConfiguration().printToLog(YADOMS_LOG(information));

      if (data.getDeviceType() == "WES")
      {
         equipment = boost::make_shared<equipments::CWESEquipment>(api, 
                                                                   data.getDeviceName(), 
                                                                   data.getConfiguration(),
                                                                   configuration);
         ioManager->addEquipment(equipment);
      }
      else if (data.getDeviceType() == "TemperatureProbe")
      {
         equipment = boost::make_shared<equipments::CtemperatureProbe>(api,
                                                                       data.getDeviceName(),
                                                                       data.getConfiguration(),
                                                                       configuration);

         //subEquipment = boost::make_shared<equipments::CWESEquipment>(api, data.getDeviceName());
         //ioManager->addEquipment(subEquipment);
      }
      else
      {
         YADOMS_LOG(error) << "no section defined for " << data.getDeviceType();
      }
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error : " << e.what();
      throw CManuallyDeviceCreationException(e.what());
   }

   return equipment->getDeviceName();
}

CWESFactory::~CWESFactory()
{}