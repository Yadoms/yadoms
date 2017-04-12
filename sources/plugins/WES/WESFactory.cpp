#include "stdafx.h"
#include "WESFactory.h"
#include "equipments/WESEquipment.h"
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
         type = api->getDeviceDetails(*devicesIterator).get<std::string>("type");

         if (type == "WES")
         {
            equipment = boost::make_shared<equipments::CWESEquipment>(api, (*devicesIterator), api->getDeviceConfiguration((*devicesIterator)));
            masterdeviceList.push_back(equipment);
            deviceList.push_back(equipment);
         }

         // TODO : For other extension
         //devicesList.push_back(equipment);

         // TODO : Add here the other creations
      }
      catch (...)
      {
      }

      YADOMS_LOG(information) << "Name : " << (*devicesIterator);
      YADOMS_LOG(information) << "Model : " << type;
   }

   return boost::make_shared<CIOManager>(deviceList, masterdeviceList);
}

std::string CWESFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                              const boost::shared_ptr<CIOManager> ioManager,
                                              const yApi::IManuallyDeviceCreationData& data)
{
   boost::shared_ptr<equipments::IEquipment> equipment;

   try {

      data.getConfiguration().printToLog(YADOMS_LOG(information));

      if (data.getConfiguration().get<bool>("type.content.WES.radio"))
      {
         equipment = boost::make_shared<equipments::CWESEquipment>(api, data.getDeviceName(), data.getConfiguration().get<shared::CDataContainer>("type.content.WES.content"));
         ioManager->addEquipment(equipment);
      }
      else if (data.getConfiguration().get<bool>("type.content.Other.radio"))
      {
         //subEquipment = boost::make_shared<equipments::CWESEquipment>(api, data.getDeviceName());
         //ioManager->addEquipment(subEquipment);
      }
   }
   catch (std::exception& e)
   {
      throw CManuallyDeviceCreationException(e.what());
   }

   return equipment->getDeviceName();
}

CWESFactory::~CWESFactory()
{}