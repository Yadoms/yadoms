#include "stdafx.h"
#include "WESFactory.h"
#include "equipments/WESEquipment.h"
#include "equipments/manuallyDeviceCreationException.hpp"
#include <shared/Log.h>

CWESFactory::CWESFactory()
{}

void CWESFactory::loadConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const boost::shared_ptr<IWESConfiguration> configuration)
{

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
            equipment = boost::make_shared<equipments::CWESEquipment>(api, (*devicesIterator));
            m_WESList.push_back(equipment);
         }

         // TODO : For other extension
         //m_devicesList.push_back(equipment);

         // TODO : Add here the other creations
      }
      catch (...)
      {
      }

      YADOMS_LOG(information) << "Name : " << (*devicesIterator);
      YADOMS_LOG(information) << "Model : " << type;
   }
}

int CWESFactory::getMasterEquipment()
{
   return m_WESList.size();
}

std::string CWESFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                              const yApi::IManuallyDeviceCreationData& data)
{
   boost::shared_ptr<equipments::IEquipment> equipment;

   try {

      if (data.getConfiguration().get<bool>("type.content.WES.radio"))
      {
         equipment = boost::make_shared<equipments::CWESEquipment>(api, data.getDeviceName());
         m_WESList.push_back(equipment);
      }
      else if (data.getConfiguration().get<bool>("type.content.Other.radio"))
      {}

      if (equipment)
         m_devicesList.push_back(equipment);
   }
   catch (std::exception& e)
   {
      throw CManuallyDeviceCreationException(e.what());
   }

   return equipment->getDeviceName();
}

void CWESFactory::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved)
{
   for (unsigned char counter = 0; counter < m_devicesList.size(); ++counter)
   {
      YADOMS_LOG(information) << "device Name : " << m_devicesList[counter]->getDeviceName() ;
      YADOMS_LOG(information) << "deviceRemoved : " << deviceRemoved ;

      // Deletion from the list of the device
      if (m_devicesList[counter]->getDeviceName() == deviceRemoved)
      {

         // If it's an extension, we delete the extension.
         // If it's the WES
         if (m_devicesList[counter]->getDeviceType() == "WES")
         {
            // remove the extension
            m_WESList.erase(m_devicesList.begin() + counter);
         }
         else
            m_devicesList.erase(m_devicesList.begin() + counter);
      }
   }

   YADOMS_LOG(information) << deviceRemoved << " was removed";
}

CWESFactory::~CWESFactory()
{}